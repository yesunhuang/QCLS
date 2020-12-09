#include "common.h"

namespace ayaji {
	LabelSeq Common::mul(const LabelSeq& label1, const LabelSeq& label2) {
		size_t len1 = label1.size();
		size_t len2 = label2.size();
		LabelSeq ret;
		if (label1.front() == 0) {
			ret.assign(label2.cbegin(), label2.cend());
			return ret;
		}
		else if (label2.front() == 0) {
			ret.assign(label1.cbegin(), label1.cend());
			return ret;
		}

		int nowIndex1, nowIndex2;
		nowIndex1 = nowIndex2 = 0;
		int nextIndex1, nextIndex2;
		nextIndex1 = nextIndex2 = 0;
		nextIndex1 = _GetNextCPIndexFromOPArray(label1, nowIndex1);
		nextIndex2 = _GetNextCPIndexFromOPArray(label2, nowIndex2);
		while (nowIndex1 < len1 && nowIndex2 < len2) {
			int cp = _CompareOPArrayBuffer(label1, nowIndex1, label2, nowIndex2);
			if (cp == 1) {
				/* buf1 > buf2 */
				for (; nowIndex2 < nextIndex2; ++nowIndex2) {
					ret.push_back(label2[nowIndex2]);
				}
				nextIndex2 = _GetNextCPIndexFromOPArray(label2, nowIndex2);
			}
			else if (cp == 0) {
				/* buf1 == buf2 */
				for (; nowIndex1 < nextIndex1; ++nowIndex1) {
					ret.push_back(label1[nowIndex1]);
				}
				nextIndex1 = _GetNextCPIndexFromOPArray(label1, nowIndex1);
				if (nowIndex1 == len1)
					ret.push_back(0);

				for (; nowIndex2 < nextIndex2; ++nowIndex2) {
					ret.push_back(label2[nowIndex2]);
				}
				nextIndex2 = _GetNextCPIndexFromOPArray(label2, nowIndex2);
			}
			else {
				/* buf1 < buf2 */
				for (; nowIndex1 < nextIndex1; ++nowIndex1) {
					ret.push_back(label1[nowIndex1]);
				}
				nextIndex1 = _GetNextCPIndexFromOPArray(label1, nowIndex1);
			}
		}

		/* 处理剩下的结点 */
		if (nowIndex1 == len1 && nowIndex2 == len2) {
			// do nothing
		}
		else if (nowIndex1 == len1) {
			if (ret.size() >= 1 && ret.back() != 0)
				ret.push_back(0);
			for (; nowIndex2 < len2; nowIndex2++) {
				ret.push_back(label2[nowIndex2]);
			}
		}
		else {
			/* nowIndex2 == len2 */
			if (ret.size() >= 1 && ret.back() != 0)
				ret.push_back(0);
			for (; nowIndex1 < len1; nowIndex1++) {
				ret.push_back(label1[nowIndex1]);
			}
		}

		return ret;
	}

	LabelSeq Common::mul_Connect(const LabelSeq& label1, const LabelSeq& label2) {
		size_t len1 = label1.size();
		size_t len2 = label2.size();
		LabelSeq ret;
		if (label1.front() == 0) {
			ret.assign(label2.cbegin(), label2.cend());
			return ret;
		}
		else if (label2.front() == 0) {
			ret.assign(label1.cbegin(), label1.cend());
			return ret;
		}

		if (label1[0] <= label2[0]) {
			for (int i = 0; i < len1; ++i)
				ret.push_back(label1[i]);
			for (int i = 0; i < len2; ++i)
				ret.push_back(label2[i]);
		}
		else {
			for (int i = 0; i < len2; ++i)
				ret.push_back(label2[i]);
			for (int i = 0; i < len1; ++i)
				ret.push_back(label1[i]);
		}
		return ret;
	}

	Label Common::maxLabel(const LabelSeq& seq) {
		Label ret = -1;
		for (auto it = seq.cbegin(); it != seq.cend(); ++it) {
			ret = ((*it) > ret) ? *it : ret;
		}
		return ret;
	}



	LabelSeq Common::barSeq(const LabelSeq& seq) {
		LabelSeq tempSeq;
		tempSeq.reserve(seq.size());
		int ops[MAX_OPERATOR_LENGTH] = { 0 };
		for (auto it = seq.cbegin(); it != seq.cend(); ++it) {
			ops[*it] += 1;
		}
		for (int i = 1; i + 1 < MAX_OPERATOR_LENGTH; i += 2) {
			int t = ops[i];
			ops[i] = ops[i + 1];
			ops[i + 1] = t;
		}
		for (int i = 1; i < MAX_OPERATOR_LENGTH; ++i) {
			for (int j = 0; j < ops[i]; ++j) {
				tempSeq.push_back(i);
			}
		}
		return std::move(tempSeq);
	}

	int Common::_GetNextCPIndexFromOPArray(const LabelSeq& seq, int now) {
		size_t size = seq.size();
		int i = now;
		for (; i < size && seq.at(i) != 0; ++i) {
			// nothing
		}
		return (i == size) ? static_cast<int>(size) : i + 1;
	}

	int Common::_CompareOPArrayBuffer(const LabelSeq& seq1, int s1, const LabelSeq& seq2, int s2) {
		size_t size1 = seq1.size();
		size_t size2 = seq2.size();
		int sn1 = s1, sn2 = s2;
		for (; sn1 < size1 && seq1.at(sn1) != 0; ++sn1) {}
		for (; sn2 < size2 && seq2.at(sn2) != 0; ++sn2) {}
		int len1 = sn1 - s1, len2 = sn2 - s2;

		/* begin comparing */
		if (len1 < len2)
			return -1;
		else if (len1 > len2)
			return 1;
		else {
			for (int i = 0; i < len1; ++i) {
				if (seq1[s1 + i] > seq2[s2 + i]) {
					return 1;
				}
				else if (seq1[s1 + i] < seq2[s2 + i]) {
					return -1;
				}
			}
			return 0;
		}
	}

}

