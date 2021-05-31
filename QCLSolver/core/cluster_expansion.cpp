#include "cluster_expansion.h"
#include <cstring>

namespace ayaji {
	OPTree ClusterExp::DeltaTree(int N) {
		/* static data */
		if (N <= 0) {
			return 0;
		}
		if (N <= STANAMIC_DTREE_SIZE) {
			/* get from dynamic table */
			if (StaticDTree::g_StanamicDTrees[N] != NULL) {
				return *(StaticDTree::g_StanamicDTrees[N]);
			}
		}
		if (N <= STATIC_DTREE_SIZE) {
			// get from static method
			OPTree* t = new OPTree();
			OPTree tmp = StaticDTree::CreateStaticDTree(N);
			t->shallowCopy(tmp);
			StaticDTree::g_StanamicDTrees[N] = t;
			return *t;
		}

		int n = N - 1;
		/* create target tree */
		OPTree ret;
		ret.setChildSize(N);
		Label buf[MAX_LABEL_BUFFER_SIZE];
		for (int i = 0; i <= n - 1; ++i) {
			_DT(i, N, 0, 0, buf, ret);
		}
		LabelSeq temparr;
		for (int i = 0; i < N; ++i) {
			temparr.push_back(i + 1);
		}
		ret.insert(temparr);

		if (N <= STANAMIC_DTREE_SIZE) {
			OPTree* t = new OPTree();
			(*t).shallowCopy(ret);
			StaticDTree::g_StanamicDTrees[N] = t;
			return *t;
		}
		return std::move(ret);
	}

	OPTree ClusterExp::ClusterExpansion(const LabelSeq& seq) {
		int len = static_cast<int>(seq.size());
		PTree dtree = DeltaTree(len);
		OPTree outTree = OPTree::BuildFromPTree(dtree, seq);
		DTToBT(outTree, seq);
		BTToCEBT(outTree);
		return std::move(outTree);
	}

	int ClusterExp::DTToBT(OPTree& tree, LabelSeq seq) {
		tree.eachNode(nullptr, _NegateNode);
		pNode oriNode = tree.search(seq);
		if (oriNode != nullptr) {
			oriNode->value = Complex(-1, 0) * oriNode->value;
		}
		return 0;
	}

	int ClusterExp::BTToCEBT(OPTree& tree) {
		for (int i = 0; i <= tree.childSize; ++i) {
			if (tree.root->children[i] != nullptr)
				_BTToCEBT(tree.root->children[i], tree);
		}

		return 0;
	}

	int ClusterExp::_DT(int nowi, int N, int prevIndex, int alCount, Label* buf, OPTree& outTree) {
		if (alCount == nowi) {
			Label buf1[MAX_LABEL_BUFFER_SIZE];
			Label buf2[MAX_LABEL_BUFFER_SIZE];
			memset(buf2, 0, MAX_LABEL_BUFFER_SIZE * sizeof(Label));

			/* 构建{i}和<N-i> */
			for (int i = 0; i < nowi; ++i) {
				buf2[buf[i]] = 1;
			}
			int index = 0;
			for (int i = 1; i <= N - 1; ++i) {
				if (buf2[i] == 1) {
					buf1[index] = i;
					++index;
				}
			}
			index = 0;
			for (int i = 1; i <= N - 1; ++i) {
				if (buf2[i] == 0) {
					buf2[index] = i;
					++index;
				}
			}
			buf1[nowi] = N;

			Complex tempv(-1, 0);
			OPTree newDeltaTree = DeltaTree(nowi + 1);
			OPTree newOPTree = OPTree::BuildFromPTree(newDeltaTree, LabelSeq(buf1, buf1 + (nowi + 1)));
			outTree.add(newOPTree.mul_TO(LabelSeq(buf2, buf2 + (N - nowi - 1)), tempv));

			return 0;
		}
		else {
			for (int i = prevIndex + 1; i <= N - nowi + alCount; ++i) {
				buf[alCount] = i;
				_DT(nowi, N, i, alCount + 1, buf, outTree);
			}
			return 0;
		}
	}

	int ClusterExp::_BTToCEBT(pNode& node, OPTree& tree) {
		if (!node->value.isZero() && !node->hasChildren()) {
			tree.deleteNode_r(node);
			tree.nodeCount -= 1;
			return 1;
		}
		int ret = 0;
		for (int i = 1; i <= tree.childSize; ++i) {
			if (node->children[i] != NULL) {
				ret |= _BTToCEBT(node->children[i], tree);
			}
			if (ret == 1)
				return 1;
		}
		return 0;
	}

	void ClusterExp::_NegateNode(pNode node, void* sth) {
		node->value = Complex(-1, 0) * (node->value);
	}

}
