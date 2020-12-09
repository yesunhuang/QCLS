/**
 * @ FileName: node.c
 * @ Function: implement the function of node
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 04/03/2020
 *
 */

#include "node.h"

namespace ayaji {
	Node::Node()
		: label(0)
	{
		parent = nullptr;
	}

	Node::Node(int childSize)
		: label(0)
	{
		this->children.resize(childSize + 1, nullptr);
		parent = nullptr;
	}

	Node::Node(int childSize, Label l, Complex v)
		: label(l)
	{
		this->children.resize(childSize + 1, nullptr);
		this->value = v;
		parent = nullptr;
	}

	Node::Node(int childSize, Label l, Complex v, const pNode& p)
		: label(l)
	{
		this->children.resize(childSize + 1, nullptr);
		this->value = v;
		parent = p;
	}

	Node::~Node()
	{
	}

	pNode Node::createNode() {
		return new Node();
	}

	pNode Node::createNode(int childSize) {
		return new Node(childSize);
	}

	pNode Node::createNode(int childSize, Label l, Complex v) {
		return new Node(childSize, l, v);
	}

	pNode Node::createNode(int childSize, Label l, Complex v, const pNode& parent) {
		return new Node(childSize, l, v, parent);
	}

	pNode Node::getPtr() {
		return this;
	}

	void Node::clear() {
		parent = nullptr;
		children.clear();
	}

	void Node::clear_r() {
		parent = nullptr;
		for (auto it = children.begin(); it != children.end(); ++it) {
			if ((*it) != nullptr) {
				(*it)->clear_r();
				delete (*it);
			}
		}
		children.clear();
	}

	bool Node::hasChild(int childLabel) const {
		return (childLabel > this->children.size() - 1)? false : children[childLabel] != nullptr;
	}

	bool Node::hasChildren() const {
		for (auto it = children.cbegin(); it != children.cend(); ++it) {
			if (*it != nullptr)
				return true;
		}
		return false;
	}

	bool Node::isZero() const {
		return this->value.isZero();
	}

	void Node::resize_r(int newSize) {
		if (newSize < 0)
			throw std::out_of_range("New size is out of range");
		int nowSize = static_cast<int>(children.size()) - 1;
		if (nowSize == newSize) return;
		else if (nowSize < newSize) {
			children.insert(children.cend(), newSize - nowSize, nullptr);
			for (auto it = children.begin(); it != children.end(); ++it) {
				if ((*it) != nullptr)
					(*it)->resize_r(newSize);
			}
		}
		else {
			// nowSize > newSize
			for (int i = newSize + 1; i < nowSize; ++i) {
				children[i]->clear_r();
			}
			children.resize(newSize + 1);
		}
	}

	pNode Node::getRoot() {
		pNode nowNode = this->getPtr();
		while (nowNode->parent != nullptr)
			nowNode = nowNode->parent;

		return std::move(nowNode);
	}

	LabelSeq Node::seqFromRoot() {
		Label tmpbuf[256];
		pNode nowNode = this->getPtr();
		int size = 0;
		while (nowNode != nullptr && nowNode->parent != nullptr) {
			tmpbuf[size++] = (*nowNode).label;
			nowNode = nowNode->parent;
		}

		LabelSeq s;
		s.reserve(size);
		for (int i = size - 1; i >= 0; --i) {
			s.push_back(tmpbuf[i]);
		}
		return std::move(s);
	}

	int Node::getFirstNonEmptyChildIndex(void) {
		int t = 0;
		int size = static_cast<int>(children.size());
		for (; t < size; ++t) {
			if (children[t] != nullptr)
				return t;
		}
		return -1;
	}

	pNode Node::getFirstNonEmptyChild(void) {
		int t = getFirstNonEmptyChildIndex();
		return (t == -1) ? nullptr : children[t];
	}

	int Node::getLastNonEmptyChildIndex(void) {
		int size = static_cast<int>(children.size());
		int t = size - 1;
		for (; t >= 0; --t) {
			if (children[t] != nullptr)
				return t;
		}
		return -1;
	}

	pNode Node::getLastNonEmptyChild(void) {
		int t = getLastNonEmptyChildIndex();
		return (t == -1) ? nullptr : children[t];
	}
}