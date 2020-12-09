/**
 * @ FileName: node.h
 * @ Function: declare the function of node
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 06/21/2020
 *
 */

#pragma once

#ifndef _NODE_H_
#define _NODE_H_
//#define SMARTPOINTER

#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include "complex.h"

namespace ayaji {
	class OPTree;
	using Label = int;
	typedef std::vector<Label> LabelSeq;

	class Node : public std::enable_shared_from_this<Node> {
	public:
#ifdef SMARTPOINTER
		using pNode = std::shared_ptr<Node>;
		using pNode_weak = std::weak_ptr<Node>;
#else
		using pNode = Node*;
		using pNode_weak = Node*;
#endif // SMARTPOINTER

		Node();
		// actually, space 0 is a spaceholder
		// 'childSize' refer to other children of this node
		// the actual size of 'children' is 'childSize + 1'
		Node(int childSize);
		Node(int childSize, Label l, Complex v);
		Node(int childSize, Label l, Complex v, const pNode& parent);
		~Node();
		// factory mode
		static pNode createNode();
		static pNode createNode(int childSize);
		static pNode createNode(int childSize, Label l, Complex v);
		static pNode createNode(int childSize, Label l, Complex v, const pNode& p);

		/// <summary>
		/// get pointer
		/// </summary>
		/// <returns>pointer of the tree node</returns>
		pNode getPtr();
		/// <summary>
		/// only clear the node's content, not recursive
		/// </summary>
		void clear();
		/// <summary>
		/// clear the node's content, and delete its child
		/// </summary>
		void clear_r();
		/// <summary>
		/// test whether the specific child node is not nullptr
		/// </summary>
		/// <param name="childLabel">label of child</param>
		/// <returns>if the child exist, return true; else return false.</returns>
		bool hasChild(int childLabel) const;
		/// <summary>
		/// test whether the node has exist child
		/// </summary>
		/// <returns>if has child(ren), return true; else return false.</returns>
		bool hasChildren() const;
		/// <summary>
		/// test whether the value of this node is 0
		/// </summary>
		/// <returns>if the value is 0, return true; else return false.</returns>
		bool isZero() const;
		/// <summary>
		/// change the childSize of the node and its children recursively
		/// </summary>
		/// <param name="newSize">new childSize</param>
		void resize_r(int newSize);
		/// <summary>
		/// get the toppest parent node, whose parent is null
		/// </summary>
		/// <returns>the pointer of toppest parent node</returns>
		pNode getRoot();
		/// <summary>
		/// create the sequence from the root to this node
		/// </summary>
		/// <returns>the sequence from the root to this node</returns>
		LabelSeq seqFromRoot();
		/// <summary>
		/// get first non-empty child index
		/// </summary>
		/// <returns>if existed, return first non-empty child index; else return -1.</returns>
		int getFirstNonEmptyChildIndex(void);
		/// <summary>
		/// get first non-empty child
		/// </summary>
		/// <returns>if existed, first non-empty child; else return -1.</returns>
		pNode getFirstNonEmptyChild(void);
		/// <summary>
		/// get last non-empty child index
		/// </summary>
		/// <returns>if existed, last non-empty child index; else return -1.</returns>
		int getLastNonEmptyChildIndex(void);
		/// <summary>
		/// get last non-empty child
		/// </summary>
		/// <returns>if existed, last non-empty child; else return -1.</returns>
		pNode getLastNonEmptyChild(void);

		Label label;
		Complex value;
		std::vector<pNode> children;
		pNode_weak parent;
	private:

	};

#ifdef SMARTPOINTER
	typedef std::shared_ptr<Node> pNode;
	typedef std::weak_ptr<Node> pNode_weak;
#else
	typedef Node* pNode;
	typedef Node* pNode_weak;
#endif // SMARTPOINTER

#ifdef SMARTPOINTER

#else

#endif // SMARTPOINTER
}


#endif // !_NODE_H_