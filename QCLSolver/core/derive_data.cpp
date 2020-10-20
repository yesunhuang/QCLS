/**
 * @ �ļ���: derive_data.cpp
 * @ ����: ʵ���й�DeriveData�Ĺ���
 * @ ����: AyajiLin & YesunHuang
 * @ ����: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ ����: 06/24/2020
 *
 */
#include "derive_data.h"

namespace ayaji {
	DeriveData::DeriveData(int capacity, int csize, int hoSize, int coSize) 
		: capacity(capacity), size(0), hoSize(hoSize), coSize(coSize)
	{
		curValues.resize(capacity);
		hoCoefs.resize(hoSize);
		coCoefs.resize(coSize);
		evoTrees_HO.resize(capacity);
		evoTrees_CO.resize(capacity);
		for (int i = 0; i < capacity; ++i) {
			evoTrees_HO[i].resize(hoSize);
			evoTrees_CO[i].resize(coSize);
		}
		trackNodes.resize(capacity);
		trackTree.setChildSize(csize);
		trackValueTree.setChildSize(csize);
	}

	DeriveData::DeriveData(DeriveData&& other) noexcept
		: capacity(other.capacity), size(other.size), hoSize(other.hoSize), coSize(other.coSize)
	{
		curValues.swap(other.curValues);
		hoCoefs.swap(other.hoCoefs);
		coCoefs.swap(other.coCoefs);
		evoTrees_HO.swap(other.evoTrees_HO);
		evoTrees_CO.swap(other.evoTrees_CO);
		trackNodes.swap(other.trackNodes);
		trackTree.shallowCopy(other.trackTree);
		trackValueTree.shallowCopy(other.trackValueTree);
	}

	DeriveData::~DeriveData() {

	}

	int DeriveData::insert(const LabelSeq& seq, Complex c) {
		pNode tmpNode = trackTree.search(seq);
		if (tmpNode != nullptr && !tmpNode->isZero())
			return 0;
		else if (size >= capacity) {
			reserve(__max(2 * capacity, size + 8));
		}

		curValues[this->size] = c;
		// �˴�ע��
		//for (int i = 0; i < data->hoSize; ++i)
		//	data->evoTrees_HO[data->size][i] = NULL;
		//for (int i = 0; i < data->coSize; ++i)
		//	data->evoTrees_CO[data->size][i] = NULL;
		trackTree.insert(seq);
		trackValueTree.insert(seq);
		trackNodes[size] = trackTree.search(seq);
		this->size += 1;
		return 0;
	}

	int DeriveData::reserve(int newCapacity) {
		newCapacity = __max(0, newCapacity);
		if (newCapacity == capacity)
			return 0;
		else {
			curValues.resize(newCapacity);
			evoTrees_HO.resize(newCapacity);
			evoTrees_CO.resize(newCapacity);
			for (int i = capacity; i < newCapacity; ++i) {
				evoTrees_HO[i].resize(hoSize);
				evoTrees_CO[i].resize(coSize);
			}
			trackNodes.resize(newCapacity);
			return 0;
		}
	}

	Complex DeriveData::initialValue(const LabelSeq& seq, const LabelSeq& sseq) {
		int len = seq.size();
		int slen = sseq.size();
		if (len == 0 || slen == 0) {
			return Complex(0, 0);
		}
		else if (seq[0] == 0) {
			return Complex(1, 0);
		}

		int maxn = -1;
		for (int i = 0; i < len; ++i) {
			maxn = __max(maxn, seq[i]);
		}
		if ((maxn + 1) / 2 - 1 > slen)
			return Complex(0, 0);

		int dagger = -1, normal = -1;
		int nextIndex = -1;
		/* n��ʾcreation operator����Ŀ,��dagger����Ŀ */
		/* m��ʾanihilation operator����Ŀ,��normal����Ŀ */
		int n = 0, m = 0;
		double ans = 1;
		int state = 0;
		while (state != -1) {
			switch (state) {
			case -1:
				/* ״̬-1: �˳�״̬ */
				break;
			case 0:
				/* ״̬0: ��ʼ�� */
				nextIndex = 0;
				state = 1;
				break;
			case 1:
				/* ״̬1: ����dagger��normal */
				n = m = 0;
				normal = ((seq[nextIndex] + 1) / 2) * 2;
				dagger = normal - 1;
				state = 2;
				break;
			case 2:
				/* ״̬2: ��ʼ����ͬһ�� */
				if (nextIndex >= len) {
					state = 3;
				}
				else if (seq[nextIndex] == normal) {
					++m;
					++nextIndex;
				}
				else if (seq[nextIndex] == dagger) {
					++n;
					++nextIndex;
				}
				else {
					state = 3;
				}
				break;
			case 3:
				/* ״̬3: ͳ��n��m,����ans */
			{
				if (n != m) {
					ans = 0;
					state = -1;
					break;
				}
				else {
					int n_alpha = sseq[normal / 2 - 1];
					if (n > n_alpha) {
						ans = 0;
						state = -1;
					}
					else {
						for (int i = n_alpha; i >= n_alpha - n + 1; --i) {
							ans *= i;
						}
					}

					if (nextIndex >= len)
						state = -1;
					else
						state = 1;
				}
				break;
			}
			default:
				break;
			}
		}

		return Complex(ans, 0);
	}

	std::pair<std::vector<OPTree>, std::vector<OPTree>> DeriveData::evolution(const std::vector<LabelSeq>& inputSeq_HO, const std::vector<Complex>& inputSeqCoef_HO, 
		const std::vector<LabelSeq>& inputSeq_CO, const std::vector<Complex>& inputSeqCoef_CO, 
		const LabelSeq& userSeq, int maxOPLen)
	{
		std::pair<std::vector<OPTree>, std::vector<OPTree>> ret;
		ret.first = _evolution_HO(inputSeq_HO, inputSeqCoef_HO, userSeq, maxOPLen);
		ret.second = _evolution_CO(inputSeq_CO, inputSeqCoef_CO, userSeq, maxOPLen);
		return ret;
	}

	DeriveData DeriveData::deriveAssign(const std::vector<LabelSeq>& inputSeq_HO, const std::vector<Complex>& inputSeqCoef_HO, 
		const std::vector<LabelSeq>& inputSeq_CO, const std::vector<Complex>& inputSeqCoef_CO, 
		const LabelSeq& inputSeq_Init, const std::vector<LabelSeq>& inputSeq_Track, int maxOPLen) {

		/* ����ȷ��csize,������dataʵ�� */
		Label maxInitTrackLabel = 0;
		for (int i = 0; i < inputSeq_Track.size(); ++i) {
			for (int j = 0; j < inputSeq_Track[i].size(); ++j) {
				maxInitTrackLabel = __max(maxInitTrackLabel, inputSeq_Track[i][j]);
			}
		}
		DeriveData output(2, maxInitTrackLabel, inputSeq_HO.size(), inputSeq_CO.size());

		/* ��ʼ����tracking operator, ��ͬʱ����initial value */
		for (int i = 0; i < inputSeq_Track.size(); ++i) {
			Complex tempc = { 1,0 };
			output.insert(inputSeq_Track[i], tempc);
			Complex tempc2 = DeriveData::initialValue(inputSeq_Track[i], inputSeq_Init);
			output.curValues[i] = tempc2;
		}

		/* derive�Ĺ��� */
		LabelSeq nowIndexSeq;
		for (int index = 0; index < output.size; ++index) {
			nowIndexSeq = (*output.trackNodes[index]).seqFromRoot();
			std::pair<std::vector<OPTree>, std::vector<OPTree>> p = DeriveData::evolution(
				inputSeq_HO, inputSeqCoef_HO,
				inputSeq_CO, inputSeqCoef_CO,
				nowIndexSeq, maxOPLen
			);
			output.evoTrees_HO[index] = p.first;
			output.evoTrees_CO[index] = p.second;
			for (int i = 0; i < inputSeq_HO.size(); ++i) {
				DeriveData::_DeriveAT(output.evoTrees_HO[index][i], inputSeq_Init, output);
			}
			for (int i = 0; i < inputSeq_CO.size(); ++i) {
				DeriveData::_DeriveAT(output.evoTrees_CO[index][i], inputSeq_Init, output);
			}
		}

		/* ����ϵ���Ĺ��� */
		for (int i = 0; i < inputSeq_HO.size(); ++i) {
			Complex temp = { 0, 1 };
			temp *= inputSeqCoef_HO[i];
			output.hoCoefs[i] = temp;
		}
		for (int i = 0; i < inputSeq_CO.size(); ++i) {
			Complex temp = { (double)0.5, 0 };
			temp *= inputSeqCoef_CO[i];
			output.coCoefs[i] = temp;
		}

		/* ��trackValueTree�н���value��Ϊ��current value */
		_UpdateDDTTreeValue(output);

		return std::move(output);
	}


	DeriveData* DeriveData::deriveAssign_raw(const std::vector<LabelSeq>& inputSeq_HO, const std::vector<Complex>& inputSeqCoef_HO,
		const std::vector<LabelSeq>& inputSeq_CO, const std::vector<Complex>& inputSeqCoef_CO,
		const LabelSeq& inputSeq_Init, const std::vector<LabelSeq>& inputSeq_Track, int maxOPLen) {

		/* ����ȷ��csize,������dataʵ�� */
		Label maxInitTrackLabel = 0;
		for (int i = 0; i < inputSeq_Track.size(); ++i) {
			for (int j = 0; j < inputSeq_Track[i].size(); ++j) {
				maxInitTrackLabel = __max(maxInitTrackLabel, inputSeq_Track[i][j]);
			}
		}
		DeriveData* poutput = new DeriveData(2, maxInitTrackLabel, inputSeq_HO.size(), inputSeq_CO.size());
		DeriveData& output = *poutput;

		/* ��ʼ����tracking operator, ��ͬʱ����initial value */
		for (int i = 0; i < inputSeq_Track.size(); ++i) {
			Complex tempc = { 1,0 };
			output.insert(inputSeq_Track[i], tempc);
			Complex tempc2 = DeriveData::initialValue(inputSeq_Track[i], inputSeq_Init);
			output.curValues[i] = tempc2;
		}

		/* derive�Ĺ��� */
		LabelSeq nowIndexSeq;
		for (int index = 0; index < output.size; ++index) {
			nowIndexSeq = (*output.trackNodes[index]).seqFromRoot();
			std::pair<std::vector<OPTree>, std::vector<OPTree>> p = DeriveData::evolution(
				inputSeq_HO, inputSeqCoef_HO,
				inputSeq_CO, inputSeqCoef_CO,
				nowIndexSeq, maxOPLen
			);
			output.evoTrees_HO[index] = p.first;
			output.evoTrees_CO[index] = p.second;
			for (int i = 0; i < inputSeq_HO.size(); ++i) {
				DeriveData::_DeriveAT(output.evoTrees_HO[index][i], inputSeq_Init, output);
			}
			for (int i = 0; i < inputSeq_CO.size(); ++i) {
				DeriveData::_DeriveAT(output.evoTrees_CO[index][i], inputSeq_Init, output);
			}
		}

		/* ����ϵ���Ĺ��� */
		for (int i = 0; i < inputSeq_HO.size(); ++i) {
			Complex temp = { 0, 1 };
			temp *= inputSeqCoef_HO[i];
			output.hoCoefs[i] = temp;
		}
		for (int i = 0; i < inputSeq_CO.size(); ++i) {
			Complex temp = { (double)0.5, 0 };
			temp *= inputSeqCoef_CO[i];
			output.coCoefs[i] = temp;
		}

		/* ��trackValueTree�н���value��Ϊ��current value */
		_UpdateDDTTreeValue(output);

		return poutput;
	}


	std::vector<Complex> DeriveData::calEvolution() {
		/* ������data->size�ȳ��ĸ������� */
		std::vector<Complex> output(this->size, Complex(0, 0));

		for (int i = 0; i < this->size; ++i) {
			LabelSeq seq = trackNodes[i]->seqFromRoot();
			for (int j = 0; j < this->evoTrees_HO[i].size(); ++j) {
				_CalEvo(this->evoTrees_HO[i][j], j, CalEvolutionType::H, output[i]);
			}
			for (int j = 0; j < this->evoTrees_CO[i].size(); ++j) {
				_CalEvo(this->evoTrees_CO[i][j], j, CalEvolutionType::C, output[i]);
			}
		}

		return std::move(output);
	}

	void DeriveData::_multiplyNodeWithComplex(pNode node, void* sth) {
		node->value *= *((Complex*)sth);
	}

	std::vector<OPTree> DeriveData::_evolution_HO(const std::vector<LabelSeq>& inputSeq_HO,
		const std::vector<Complex>& inputSeqCoef_HO, 
		const LabelSeq& userSeq, int maxOPLen) 
	{
		/* ho���� */

		LabelSeq buf;
		int userSeqLen = userSeq.size();
		std::vector<OPTree> ret;
		for (int i = 0; i < inputSeq_HO.size(); ++i) {
			/* �ȹ���<O_j A_i> */
			buf.clear();
			buf.insert(buf.cend(), inputSeq_HO[i].cbegin(), inputSeq_HO[i].cend());
			buf.insert(buf.cend(), userSeq.cbegin(), userSeq.cend());
			OPTree OA_Tree = OPTree::MONormalize(buf);
			//PrintOPTree(OA_Tree);

			/* �ٹ���<A_i O_j> */
			buf.clear();
			buf.insert(buf.cend(), userSeq.cbegin(), userSeq.cend());
			buf.insert(buf.cend(), inputSeq_HO[i].cbegin(), inputSeq_HO[i].cend());
			OPTree AO_Tree = OPTree::MONormalize(buf);
			//PrintOPTree(AO_Tree);

			/* ������� */
			AO_Tree.eachNode(nullptr, ClusterExp::_NegateNode);
			OA_Tree.add(AO_Tree);
			//PrintOPTree(OA_Tree);

			/* ȥ�����ȳ�����Χ�� */
			/* TODO: ��������ܴ�,�պ���Ҫ�Ż� */
			while (_DeleteAndCE(OA_Tree, maxOPLen) == 1) {}

			ret.push_back(OA_Tree);
		}
		return ret;
	}

	std::vector<OPTree> DeriveData::_evolution_CO(const std::vector<LabelSeq>& inputSeq_CO, 
		const std::vector<Complex>& inputSeqCoef_CO, 
		const LabelSeq& userSeq, int maxOPLen)
	{
		/* co���� */
		//UINT_L dagger[MAX_OPERATOR_LENGTH];
		//UINT_L buf[3 * MAX_OPERATOR_LENGTH];
		LabelSeq dagger;
		LabelSeq buf;

		Complex tempc(2, 0);

		int inputSeqLen = inputSeq_CO.size();
		std::vector<OPTree> ret;
		ret.resize(inputSeqLen);
		for (int i = 0; i < inputSeqLen; ++i) {
			/* �ȹ���O_n^{\dagger} */
			dagger.clear();
			int inputSeqLens = inputSeq_CO[i].size();
			for (int j = 0; j < inputSeqLens; ++j) {
				Label t = ((inputSeq_CO[i][j] - 1) ^ 1) + 1;
				dagger.push_back(t);
			}

			/* �ȹ���<O_n^{\dagger} A_i O_n> */
			buf.clear();
			buf.insert(buf.cend(), dagger.cbegin(), dagger.cend());
			buf.insert(buf.cend(), userSeq.cbegin(), userSeq.cend());
			buf.insert(buf.cend(), inputSeq_CO[i].cbegin(), inputSeq_CO[i].cend());
			OPTree OdAO_Tree = OPTree::MONormalize(buf);
			OdAO_Tree.eachNode(&tempc, _multiplyNodeWithComplex);

			/* �ٹ���<O_n^{\dagger} O_n A_i> */
			buf.clear();
			buf.insert(buf.cend(), dagger.cbegin(), dagger.cend());
			buf.insert(buf.cend(), inputSeq_CO[i].cbegin(), inputSeq_CO[i].cend());
			buf.insert(buf.cend(), userSeq.cbegin(), userSeq.cend());
			OPTree OdOA_Tree = OPTree::MONormalize(buf);

			/* �ٹ���<A_i O_n^{\dagger} O_n> */
			buf.clear();
			buf.insert(buf.cend(), userSeq.cbegin(), userSeq.cend());
			buf.insert(buf.cend(), dagger.cbegin(), dagger.cend());
			buf.insert(buf.cend(), inputSeq_CO[i].cbegin(), inputSeq_CO[i].cend());
			OPTree AOdO_Tree = OPTree::MONormalize(buf);

			/* ������� */
			OdOA_Tree.eachNode(nullptr, ClusterExp::_NegateNode);
			AOdO_Tree.eachNode(nullptr, ClusterExp::_NegateNode);
			OdAO_Tree.add(OdOA_Tree);
			OdAO_Tree.add(AOdO_Tree);

			/* ȥ�����ȳ�����Χ�� */
			/* TODO: ��������ܴ�,�պ���Ҫ�Ż� */
			while (_DeleteAndCE(OdAO_Tree, maxOPLen) == 1) {}

			ret[i] = OdAO_Tree;
		}

		return ret;
	}

	int DeriveData::_DeleteAndCE(OPTree& tree, int maxOPLen) {
		for (auto it = tree.getIterator(); *it != nullptr; it.next()) {
			LabelSeq seq = it.getCurLabelSeq();
			int len = seq.size();
			int nowIndex = 0, nextIndex = Common::_GetNextCPIndexFromOPArray(seq, 0);
			OPTree tmpAddToNow;
			tmpAddToNow.setChildSize(tree.getChildSize());
			std::vector<std::pair<int, int>> overs = _getOverLengthIndex(seq, maxOPLen);
			int overLen = overs.size();
			int prevTailIndex = 0;
			if (overLen != 0) {
				for (int i = 0; i < overLen; ++i) {
					std::pair<int, int>& pair = overs[i];
					if (prevTailIndex < pair.first) {
						// need to multiply other ops
						if (tmpAddToNow.getNodeCount() == 0) {
							tmpAddToNow.insert(LabelSeq(seq.cbegin() + prevTailIndex, seq.cbegin() + pair.first - 1), Complex(1, 0));
						}
						else {
							tmpAddToNow = tmpAddToNow.mul_TO(LabelSeq(seq.cbegin() + prevTailIndex, seq.cbegin() + pair.first - 1), Complex(1, 0));
						}
					}

					if (pair.second >= len) {
						auto cl = ClusterExp::ClusterExpansion(LabelSeq(seq.cbegin() + pair.first, seq.cbegin() + pair.second));
						if (tmpAddToNow.getNodeCount() == 0) {
							tmpAddToNow = cl;
						}
						else {
							tmpAddToNow = tmpAddToNow.mul_TT(cl);
						}
					}
					else {
						auto cl = ClusterExp::ClusterExpansion(LabelSeq(seq.cbegin() + pair.first, seq.cbegin() + pair.second - 1));
						if (tmpAddToNow.getNodeCount() == 0) {
							tmpAddToNow = cl;
						}
						else {
							tmpAddToNow = tmpAddToNow.mul_TT(cl);
						}
						if (i == overLen - 1) {
							// the last one
							if (tmpAddToNow.getNodeCount() == 0) {
								tmpAddToNow.insert(LabelSeq(seq.cbegin() + pair.second, seq.cend()), Complex(1, 0));
							}
							else {
								tmpAddToNow = tmpAddToNow.mul_TO(LabelSeq(seq.cbegin() + pair.second, seq.cend()), Complex(1, 0));
							}
						}
					}
					prevTailIndex = pair.second;
				}
				// multiply the origin coef
				Complex originCoef = it.getNode()->value;
				tmpAddToNow.eachNode(&originCoef, _multiplyNodeWithComplex);

				tree.remove(seq);
				tree.add(tmpAddToNow);
				return 1;
			}
		}
		return 0;
	}

	std::vector<std::pair<int, int>> DeriveData::_getOverLengthIndex(const LabelSeq& seq, int maxOPLen) {
		int len = seq.size();
		int nowIndex = 0, nextIndex = Common::_GetNextCPIndexFromOPArray(seq, 0);
		std::vector<std::pair<int, int>> ret;
		while (nowIndex < len) {
			if (nextIndex >= len) {
				// reach tail
				if (nextIndex - nowIndex > maxOPLen) {
					ret.push_back(std::pair<int, int>(nowIndex, nextIndex));
				}
			}
			else {
				// reach middle
				if (nextIndex - nowIndex - 1 > maxOPLen) {
					ret.push_back(std::pair<int, int>(nowIndex, nextIndex));
				}
			}
			nowIndex = nextIndex;
			nextIndex = Common::_GetNextCPIndexFromOPArray(seq, nowIndex);
		}
		return std::move(ret);
	}

	int DeriveData::_DeriveAT(OPTree& evoTree, const LabelSeq& inputSeq_Init, DeriveData& data) {
		for (auto it = evoTree.getIterator(); *it != nullptr; it.next()) {
			if ((*it.getNode()).hasChildren()) continue;
			LabelSeq seq = it.getCurLabelSeq();
			int len = seq.size();
			if (len <= 0 || seq[0] == 0) continue;

			int now = 0;
			int next = Common::_GetNextCPIndexFromOPArray(seq, now);
			while (now < len) {
				if (next != len) {
					LabelSeq single;
					single.assign(seq.begin() + now, seq.begin() + next - 1);
					
					if (!DeriveData::_DeriveIsFind(data, single)) {
						Complex tempc = { 1, 0 };
						data.insert(single, tempc);
						Complex tempc2 = DeriveData::initialValue(single, inputSeq_Init);
						data.curValues[data.size - 1] = tempc2;
					}
					now = next;
					next = Common::_GetNextCPIndexFromOPArray(seq, now);
				}
				else {
					LabelSeq single;
					single.assign(seq.begin() + now, seq.end());

					if (!DeriveData::_DeriveIsFind(data, single)) {
						Complex tempc = { 1, 0 };
						data.insert(single, tempc);
						Complex tempc2 = DeriveData::initialValue(single, inputSeq_Init);
						data.curValues[data.size - 1] = tempc2;
					}

					break;
				}
			}
		}

		return 0;
	}

	bool DeriveData::_DeriveIsFind(DeriveData& data, const LabelSeq& seq) {
		/* Ѱ��ԭ�� */
		pNode temp = data.trackTree.search(seq);
		if (temp != nullptr && !temp->isZero()) {
			return true;
		}
		/* Ѱ�ҹ����� */
		LabelSeq tempSeq = Common::barSeq(seq);
		pNode temp2 = data.trackTree.search(tempSeq);
		if (temp2 != nullptr && !temp2->isZero()) {
			return true;
		}
		return false;
	}

	int DeriveData::_UpdateDDTTreeValue(DeriveData& data) {
		for (int i = 0; i < data.size; ++i) {
			LabelSeq s = (*data.trackNodes[i]).seqFromRoot();
			auto tempnode = data.trackValueTree.search(s);
			tempnode->value = data.curValues[i];
		}
		return 0;
	}

	int DeriveData::_CalEvo(OPTree& evoTree, int treeIndex, CalEvolutionType HorC, Complex& sum) {
		// UINT_L buf[MAX_OPERATOR_LENGTH];
		for (auto it = evoTree.getIterator(); *it != nullptr; it.next()) {
			LabelSeq seq = it.getCurLabelSeq();
			if (seq[0] == 0) {
				/* ����� */
				switch (HorC)
				{
				case CalEvolutionType::H:
					sum += this->hoCoefs[treeIndex] * it.getNode()->value;
					break;
				case CalEvolutionType::C:
					sum += this->coCoefs[treeIndex] * it.getNode()->value;
					break;
				default:
					break;
				}
			}
			else {
				/* ������� */
				__CalEvo(*it, evoTree, treeIndex, HorC, seq, sum);
			}
		}

		return 0;
	}

	int DeriveData::__CalEvo(const pNode& pnode, OPTree& evoTree, int treeIndex, 
		CalEvolutionType HorC, const LabelSeq& seq, Complex& sum) {

		int len = seq.size();
		int head = 0;
		int tail = Common::_GetNextCPIndexFromOPArray(seq, head);
		Complex tempc = { 1,0 };
		LabelSeq seq_segment;
		seq_segment.reserve(MAX_OPERATOR_LENGTH);
		while (head < len) {
			Complex tempnodev = { 0,0 };
			pNode tempnode = nullptr;

			if (tail == len) {
				seq_segment.assign(seq.begin() + head, seq.end());
			}
			else {
				seq_segment.assign(seq.begin() + head, seq.begin() + tail - 1);
			}
			tempnode = this->trackTree.search(seq_segment);
			if (tempnode != nullptr && !tempnode->isZero()) {
				tempnode = this->trackValueTree.search(seq_segment);
				tempnodev = tempnode->value;
			}
			else {
				/* dagger�Ĺ������� */
				auto bar_seq = Common::barSeq(seq_segment);
				tempnode = this->trackValueTree.search(bar_seq);
				tempnodev = tempnode->value;
				tempnodev.setImage(-tempnodev.getImage());
			}

			tempc *= tempnodev;
			head = tail;
			tail = Common::_GetNextCPIndexFromOPArray(seq, head);

		}

		switch (HorC)
		{
		case CalEvolutionType::H:
			tempc *= this->hoCoefs[treeIndex];
			break;
		case CalEvolutionType::C:
			tempc *= this->coCoefs[treeIndex];
			break;
		default:
			break;
		}

		tempc *= pnode->value;
		sum += tempc;

		return 0;
	}

}