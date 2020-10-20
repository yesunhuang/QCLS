/**
 * @ FileName: operator_tree_iterator.h
 * @ Function: implement the iterator of operator tree
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 06/21/2020
 *
 */
#include "operator_tree_iterator.h"

namespace ayaji {
	OPTree_Iterator::OPTree_Iterator(const OPTree& tree) {
		seqStack.clear();
		seqStack.reserve(32);
		nodeStack.clear();
		nodeStack.reserve(32);
		this->tree = &tree;
		if (tree.nodeCount <= 0 || tree.root == nullptr) {
			isTail = true;
			cur = nullptr;
		}

		else {
			isTail = false;
			// get to the leftmost child
			pNode nowNode = &(*tree.root);
			pNode tmp = nullptr;
			seqStack.push_back(nowNode->label);
			nodeStack.push_back(nowNode);
			while ((tmp = nowNode->getFirstNonEmptyChild()) != nullptr) {
				seqStack.push_back(tmp->label);
				nodeStack.push_back(tmp);
				nowNode = tmp;
			}
			cur = nowNode;

			if (cur->value.isZero()) {
				next();
			}
		}
	}

	OPTree_Iterator::OPTree_Iterator(OPTree_Iterator&& other) noexcept
		: isTail(other.isTail), tree(other.tree)
	{
		this->cur = other.cur;
		this->nodeStack.swap(other.nodeStack);
		this->seqStack.swap(other.seqStack);
	}
	
	pNode OPTree_Iterator::next(void) {
		if (isTail) return nullptr;
		pNode ret = this->cur;

		pNode childNode;
		Label childLabel;
		pNode nowNode;
		Label nowLabel;
		while (true) {
			childNode = nodeStack.back();
			childLabel = seqStack.back();
			nodeStack.pop_back();
			seqStack.pop_back();
			if (nodeStack.empty() && seqStack.empty()) {
				isTail = true;
				return this->cur = nullptr;
			}
			nowNode = nodeStack.back();
			nowLabel = seqStack.back();

			for (int i = childLabel + 1; i <= tree->childSize; ++i) {
				if (nowNode->hasChild(i)) {
					// go deep into the leftmost child
					nowNode = nowNode->children[i];
					pNode tmp = nullptr;
					seqStack.push_back(i);
					nodeStack.push_back(nowNode);
					while ((tmp = nowNode->getFirstNonEmptyChild()) != nullptr) {
						seqStack.push_back(tmp->label);
						nodeStack.push_back(tmp);
						nowNode = tmp;
					}
					cur = nowNode;

					if (!cur->value.isZero())
						return std::move(ret);
					else {
						next();
						return cur;
					}
				}
			}
			// no other child, choose this one
			// if this one is root, return nullptr
			if (nowNode == &(*tree->root)) {
				isTail = true;
				cur = nullptr;
				return nullptr;
			}
			else if (nowNode->value.isZero()) {
				// go on next cycle
			}
			else {
				// nowNode != tree->root && !nowNode->value.isZero()
				cur = nowNode;
				return cur;
			}

		}
	}

	int OPTree_Iterator::remove(void) {
		// TODO
		return 0;
	}

	pNode OPTree_Iterator::getNode(void) {
		return this->cur;
	}

	pNode OPTree_Iterator::operator*(void) {
		return std::move(getNode());
	}

	LabelSeq OPTree_Iterator::getCurLabelSeq(void) {
		LabelSeq s;
		s.assign(seqStack.cbegin() + 1, seqStack.cend());
		return std::move(s);
	}
}
