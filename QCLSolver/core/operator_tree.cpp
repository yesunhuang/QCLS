/**
 * @ FileName: operator_tree.c
 * @ Function: implement the function of operator tree
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 04/08/2020
 *
 */

#include "operator_tree.h"
#include "deb.h"
#include <cstring>
#include <cmath>

namespace ayaji {
	OPTree OPTree::SONormalize(const LabelSeq& seq) {
		int len = static_cast<int>(seq.size());
		/* mark */
		Label normal = ((seq[0] + 1) / 2) * 2;
		Label dagger = normal - 1;

		Label* buf = (Label*)malloc(sizeof(Label) * ((len / 2) + 1));
		if (buf == NULL) return 0;
		int zeroNum = 0;
		memset(buf, 0, sizeof(Label) * ((len / 2) + 1));
		buf[0] = 1;
		if (seq[len - 1] == dagger)
			zeroNum = 1;
		for (int i = len - 2; i >= 0; --i) {
			if (seq[i] == dagger) {
				++zeroNum;
			}
			else {
				/* arr[i] == normal */
				for (int j = std::min(zeroNum - 1, (len / 2)); j >= 0; --j) {
					buf[j + 1] += buf[j] * (zeroNum - j);
				}
			}
		}

		/* build the tree */
		OPTree ret;
		ret.setChildSize(Common::maxLabel(seq));
		LabelSeq tempbuf(len, 0);
		for (int i = 0; i < zeroNum; ++i) {
			tempbuf[i] = dagger;
		}
		for (int i = zeroNum; i < len; ++i) {
			tempbuf[i] = normal;
		}
		Complex tempv;
		tempv.setReal(buf[0]);
		ret.insert(tempbuf, tempv);
		for (int i = 1; i <= (len / 2); ++i) {
			// int arrlen = len - 2 * i;
			if (buf[i] != 0) {
				if (2 * i == len) {
					/* 0次项 */
					tempbuf[0] = 0;
					tempv.setReal(buf[i]);
					LabelSeq s;
					s.assign(tempbuf.cbegin(), tempbuf.cbegin() + 1);
					ret.insert(s, tempv);
				}
				else {
					tempbuf[zeroNum - i] = normal;
					tempv.setReal(buf[i]);
					LabelSeq s;
					s.assign(tempbuf.cbegin(), tempbuf.cbegin() + (len - 2 * i));
					ret.insert(s, tempv);
				}
			}
		}
		free(buf);
		return std::move(ret);
	}

	OPTree OPTree::MONormalize(const LabelSeq& seq_in) {
		/* 默认输入符合格式,即无0 */
		LabelSeq seq(seq_in);
		_SortMOArray(seq);
		int len = static_cast<int>(seq.size());
		int csize = Common::maxLabel(seq);
		/* m为一个mode的系底 */
		OPTree ret;
		Label m = (seq[0] + 1) / 2;
		int start = 0, end = 0;
		/* 确定第一棵 */
		for (; end < len; ++end) {
			if (end == len - 1) {
				ret = SONormalize(seq);
				return std::move(ret);
			}
			else if ((seq[end + 1] != m * 2) && (seq[end + 1] != m * 2 - 1)) {
				LabelSeq t(end + 1, 0);
				t.assign(seq.cbegin(), seq.cbegin() + end + 1);
				ret = SONormalize(t);
				start = end = end + 1;
				m = (seq[start] + 1) / 2;
				break;
			}
		}
		/* 后续的乘树 */
		while (1) {
			if (end == len - 1) {
				LabelSeq t(end - start + 1);
				t.assign(seq.cbegin() + start, seq.cend());
				OPTree tmpTree = SONormalize(t);
				ret = ret.mul_Connect_TT(tmpTree);
				return std::move(ret);
			}
			else if ((seq[end + 1] != m * 2) && (seq[end + 1] != m * 2 - 1)) {
				LabelSeq t(end - start + 1);
				t.assign(seq.cbegin() + start, seq.cbegin() + end + 1);
				OPTree tmpTree = SONormalize(t);
				ret = ret.mul_Connect_TT(tmpTree);
				start = end = end + 1;
				m = (seq[start] + 1) / 2;
			}
			else {
				++end;
			}
		}

		return ret;
	}

	OPTree::OPTree()
		: childSize(0), nodeCount(0)
	{
		root = std::shared_ptr<Node>(Node::createNode(), OPTree::_free_node);
	}

	OPTree::OPTree(int childSize)
		: childSize(childSize), nodeCount(0)
	{
		root = std::shared_ptr<Node>(Node::createNode(childSize), OPTree::_free_node);
	}

	OPTree::OPTree(const OPTree& other) {
		shallowCopy(other);
	}

	OPTree::OPTree(OPTree&& other) noexcept
		: childSize(other.childSize), nodeCount(0)
	{
		shallowCopy(other);

	}

	OPTree::~OPTree() {

	}
	
	int OPTree::add(const OPTree& other) {
		for (auto it = other.getIterator(); *it != nullptr; it.next()) {
			this->insert(it.getCurLabelSeq(), it.getNode()->value);
		}
		return 0;
	}

	int OPTree::copy(const OPTree& other) {
		clear();
		return add(other);
	}

	void OPTree::clear() {
		if (root != nullptr)
			root->clear_r();
		root = nullptr;
		childSize = 0;
		nodeCount = 0;
	}

	void OPTree::eachNode(void* sth, std::function<void(pNode node, void* sth)> func) {
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			func(*it, sth);
		}
	}

	int OPTree::insert(const LabelSeq& nodeSeq) {
		return insert(nodeSeq, Complex(1, 0));
	}

	int OPTree::insert(const LabelSeq& nodeSeq, Complex coef) {
		if (nodeSeq.size() == 0) return -1;
		else if (coef.isZero()) return 1;

		// scan the 'nodeSeq" and decide the childsize
		Label maxLabel = Common::maxLabel(nodeSeq);
		if (maxLabel > this->childSize)
			setChildSize(maxLabel);

		if (root == nullptr) {
			// create root
			root = std::shared_ptr<Node>(Node::createNode(childSize, 0, Complex()), OPTree::_free_node);
		}

		if (nodeSeq[0] == 0) {
			// insert zero order term
			if (!root->hasChild(0)) {
				// create the node
				root->children[0] = Node::createNode(childSize, 0, Complex(), &(*root));
			}
			pNode temp = root->children[0];
			/* check whether to update the nodeCount */
			nodeCount = (temp->value.isZero()) ? nodeCount + 1 : nodeCount;
			temp->value += coef;
			// delete node whose value is zero
			if (temp->value.isZero()) {
				deleteNode_r(temp);
				nodeCount -= 1;
			}
			return 0;
		}

		pNode nowNode = &(*root);
		for (auto it = nodeSeq.cbegin(); it != nodeSeq.cend(); ++it) {
			Label nowLabel = *it;
			if (!nowNode->hasChild(nowLabel)) {
				// create the node
				nowNode->children[nowLabel] = Node::createNode(childSize, nowLabel, Complex(0, 0), nowNode);
			}
			nowNode = nowNode->children[nowLabel];
		}
		/* check whether to update the nodeCount */
		nodeCount = (nowNode->value.isZero()) ? nodeCount + 1 : nodeCount;
		nowNode->value += coef;
		// delete node whose value is zero
		if (nowNode->value.isZero()) {
			deleteNode_r(nowNode);
			nodeCount -= 1;
		}

		return 0;
	}

	int OPTree::move(OPTree& other) {
		int t = add(other);
		other.clear();
		return t;
	}

	OPTree OPTree::mul_TO(const pNode& node) {
		OPTree tmp;
		LabelSeq seq = node->seqFromRoot();
		Complex coef = node->value;
		if (this->nodeCount == 0) {
			tmp.insert(seq, coef);
			return std::move(tmp);
		}
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			LabelSeq ts = Common::mul(it.getCurLabelSeq(), seq);
			tmp.insert(ts, coef * it.getNode()->value);
		}
		return std::move(tmp);
	}

	OPTree OPTree::mul_TO(const LabelSeq& seq, Complex coef) {
		OPTree tmp;
		if (this->nodeCount == 0) {
			tmp.insert(seq, coef);
			return std::move(tmp);
		}
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			LabelSeq ts = Common::mul(it.getCurLabelSeq(), seq);
			tmp.insert(ts, coef * it.getNode()->value);
		}
		return std::move(tmp);
	}

	OPTree OPTree::mul_TT(const OPTree& other) {
		OPTree tmp;
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			LabelSeq t1Seq = it.getCurLabelSeq();
			Complex t1Coef = it.getNode()->value;
			for (auto it2 = other.getIterator(); *it2 != nullptr; it2.next()) {
				LabelSeq t2Seq = it2.getCurLabelSeq();
				LabelSeq ts = Common::mul(t1Seq, t2Seq);
				tmp.insert(ts, t1Coef * it2.getNode()->value);
			}
		}
		return std::move(tmp);
	}

	OPTree OPTree::mul_Connect_TO(const pNode& node) {
		OPTree tmp;
		LabelSeq seq = node->seqFromRoot();
		Complex coef = node->value;
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			LabelSeq ts = Common::mul_Connect(it.getCurLabelSeq(), seq);
			tmp.insert(ts, coef * it.getNode()->value);
		}
		return std::move(tmp);
	}

	OPTree OPTree::mul_Connect_TT(const OPTree& other) {
		OPTree tmp;
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			LabelSeq t1Seq = it.getCurLabelSeq();
			Complex t1Coef = it.getNode()->value;
			for (auto it2 = other.getIterator(); *it2 != nullptr; it2.next()) {
				LabelSeq t2Seq = it2.getCurLabelSeq();
				LabelSeq ts = Common::mul_Connect(t1Seq, t2Seq);
				tmp.insert(ts, t1Coef * it2.getNode()->value);
			}
		}
		return std::move(tmp);
	}

	int OPTree::remove(const LabelSeq& nodeSeq) {
		pNode nowNode = &(*root);
		for (auto it = nodeSeq.cbegin(); it != nodeSeq.cend(); ++it) {
			Label nowLabel = *it;
			if (!nowNode->hasChild(nowLabel)) {
				// node not exist
				return 0;
			}
			nowNode = nowNode->children[nowLabel];
		}
		if (nowNode->value.isZero())
			// node's value is zero
			return 0;
		// node's value is not zero
		if (nowNode->hasChildren()) {
			// not the leaf node
			nowNode->value = Complex(0, 0);
			--nodeCount;
			return 0;
		}
		// the leaf node
		deleteNode_r(nowNode);
		nodeCount -= 1;
		return 0;
	}

	pNode OPTree::search(const LabelSeq& nodeSeq) const {
		pNode nowNode = &(*root);
		for (auto it = nodeSeq.cbegin(); it != nodeSeq.cend(); ++it) {
			Label nowLabel = *it;
			if (!nowNode->hasChild(nowLabel)) {
				// node not exist
				return nullptr;
			}
			nowNode = nowNode->children[nowLabel];
		}
		return std::move(nowNode);
	}

	int OPTree::setChildSize(int childSize) {
		if (childSize < 0)
			return 1;
		this->childSize = childSize;
		if (this->root == nullptr) {
			// build root
			root = std::shared_ptr<Node>(Node::createNode(childSize, 0, Complex()), OPTree::_free_node);
			return 0;
		}
		this->root->resize_r(childSize);
		return normalize();
	}

	int OPTree::shallowCopy(const OPTree& other) {
		this->root = other.root;
		this->childSize = other.childSize;
		this->nodeCount = other.nodeCount;
		return 0;
	}

	int OPTree::swap(OPTree& other) {
		auto t = other.root;
		other.root = this->root;
		this->root = t;
		auto t2 = other.childSize;
		other.childSize = this->childSize;
		this->childSize = t2;
		auto t3 = other.nodeCount;
		other.nodeCount = this->nodeCount;
		this->nodeCount = t3;
		return 0;
	}

	OPTree& OPTree::operator=(const OPTree& other) {
		this->shallowCopy(other);
		return *this;
	}

	OPTree& OPTree::operator=(OPTree&& other) {
		this->shallowCopy(other);
		return *this;
	}

	OPTree_Iterator OPTree::getIterator(void) const{
		return std::move(OPTree_Iterator(*this));
	}

	int OPTree::getChildSize() const {
		return this->childSize;
	}

	int OPTree::getNodeCount() const {
		return this->nodeCount;
	}

	OPTree OPTree::BuildFromPTree(PTree& posTree, const LabelSeq& seq) {
		int opCsize = Common::maxLabel(seq);
		OPTree outputOPTree;
		outputOPTree.setChildSize(opCsize);
		_BuildFromPTree(&(*posTree.root), posTree.childSize, seq, &(*outputOPTree.root), opCsize);
		//deb_printTree(outputOPTree);
		outputOPTree.normalize();

		return outputOPTree;
	}

	int OPTree::_SortMOArray(LabelSeq& seq) {
		/* 冒泡排序,可stable */
		/* 默认输入妹有0,且符合格式 */
		int len = static_cast<int>(seq.size());
		int lastChange = 0;
		int isChange = 1;
		while (lastChange < len - 1) {
			if (!isChange)
				return 0;
			isChange = 0;
			int tempLastChange = lastChange;
			for (int i = len - 1; i > lastChange; --i) {
				if (((seq[i] + 1) / 2) < ((seq[i - 1] + 1) / 2)) {
					isChange = 1;
					tempLastChange = i;
					// swap
					Label t = seq[i];
					seq[i] = seq[i - 1];
					seq[i - 1] = t;
				}
			}
			lastChange = tempLastChange;
		}
		return 0;
	}

	int OPTree::normalize() {
		// remove surplus node
		std::vector<pNode> v;
		std::vector<pNode> stack;
		pNode proot = &(*this->root);
		stack.push_back(proot);
		while (!stack.empty()) {
			pNode p = stack.back();
			stack.pop_back();
			for (auto it = p->children.begin(); it != p->children.end(); ++it) {
				pNode& t = *it;
				if (t != nullptr) {
					stack.push_back(t);
				}
			}

			if (!p->hasChildren() && p->value.isZero() && p != proot) {
				v.push_back(p);
			}
		}

		for (auto it = v.begin(); it != v.end(); ++it) {
			deleteNode_r(*it);
		}

		// count
		nodeCount = 1;
		for (auto it = getIterator(); *it != nullptr; it.next()) {
			nodeCount += 1;
		}
		nodeCount -= 1;
		return 0;
	}

	int OPTree::deleteNode_r(pNode& node) {
		pNode nowNode = node;
		pNode father = node->parent;
		if (nowNode->hasChildren()) return 1;
		else if (father == nullptr) {
			// this node is root and has no child
			return 0;
		}

		// this node has no child
		Label label = nowNode->label;
		father->children[label] = nullptr;
		nowNode->parent = nullptr;

		if (father != nullptr && father->isZero())
			deleteNode_r(father);
		delete nowNode;
		return 0;
	}

	void OPTree::_free_node(pNode p)
	{
		p->clear_r();
		delete p;
	}

	int OPTree::_BuildFromPTree(pNode posNode, int posCsize, LabelSeq seq, pNode opNode, int opCsize) {
		int ret = 1;
		int len = static_cast<int>(seq.size());
		for (int i = 0; i <= posCsize; ++i) {
			if (posNode->children[i] != nullptr) {
				/* 不允许位置树指向operator不存在的位置 */
				if (posNode->children[i]->label > len)
					return 0;
				Label nextChild = (posNode->children[i]->label == 0) ? 0 : seq[posNode->children[i]->label - 1];
				/* 判断该子节点是否已经建立 */
				if (opNode->children[nextChild] == nullptr) {
					Complex tempv;
					opNode->children[nextChild] = Node::createNode(opCsize, nextChild, tempv, opNode);
				}

				opNode->children[nextChild]->value += posNode->children[i]->value;
				_BuildFromPTree(posNode->children[i], posCsize, seq, opNode->children[nextChild], opCsize);
			}
		}

		return ret;
	}

}
