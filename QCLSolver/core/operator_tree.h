/**
 * @ FileName: operator_tree.h
 * @ Function: declare the function of operator tree
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 06/21/2020
 *
 */

#pragma once
#ifndef _OPERATOR_TREE_H_
#define _OPERATOR_TREE_H_

#include <functional>

#include "node.h"
#include "operator_tree_iterator.h"
#include "common.h"

#include "deb.h"

namespace ayaji {
	class OPTree_Iterator;

	class OPTree {
	public:
		using PTree = OPTree;
		// static method

		/// <summary>
		/// normalize a simple operator, and return a new tree
		/// </summary>
		/// <returns>a new tree storing the normalized labelseq</returns>
		static OPTree SONormalize(const LabelSeq& seq);
		/// <summary>
		/// normalize a multi-operator, and return a new tree
		/// </summary>
		/// <returns>a new tree storing the normalized labelseq</returns>
		static OPTree MONormalize(const LabelSeq& seq);

		OPTree();
		OPTree(int childSize);
		/// <summary>
		/// shallow copy
		/// </summary>
		/// <param name="other">trees to be copied from</param>
		/// <returns></returns>
		OPTree(const OPTree& other);
		OPTree(OPTree&& other) noexcept;
		~OPTree();

		friend class OPTree_Iterator;
		friend class StaticDTree;
		friend class ClusterExp;

		friend void deb_PrintTree(const OPTree& tree);
		friend void deb_PrintTree_(pNode& node, const LabelSeq& seqBuf, int cSize);
		template<typename T>
		friend void deb_PrintTree_(pNode& node, LabelSeq& seqBuf, int cSize, T& outputer);
		template<typename T>
		friend void deb_PrintTree(const OPTree& tree, T& func);
		friend std::vector<std::pair<Complex, LabelSeq>> deb_Tree2Pair(const OPTree& tree);
		friend void deb_Tree2Pair_(pNode& node, LabelSeq& seqBuf, int cSize, std::vector<std::pair<Complex, LabelSeq>>& pairs);

		/// <summary>
		/// Add every elements in 'other' to this
		/// </summary>
		/// <param name="other">OPTree to be moved from.</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int add(const OPTree& other);
		/// <summary>
		/// Deep copy of OPTress
		/// </summary>
		/// <param name="other">OPTree to be copied from.</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int copy(const OPTree& other);
		/// <summary>
		/// clear or reset the tree
		/// </summary>
		void clear();
		/// <summary>
		/// do function on each node
		/// </summary>
		/// <param name="sth">some param</param>
		/// <param name="func">function to be done on each node</param>
		void eachNode(void* sth, std::function<void(pNode node, void* sth)> func);
		/// <summary>
		/// insert a node sequence as node into the tree. Its coefficent is default 1.
		/// </summary>
		/// <param name="nodeSeq">node sequence</param>
		/// <returns></returns>
		int insert(const LabelSeq& nodeSeq);
		/// <summary>
		/// insert a node sequence as node into the tree.
		/// </summary>
		/// <param name="nodeSeq">node sequence</param>
		/// <param name="coef">coefficient</param>
		/// <returns></returns>
		int insert(const LabelSeq& nodeSeq, Complex coef);
		/// <summary>
		/// Move every elements in 'other' to this
		/// </summary>
		/// <param name="other">OPTree to be moved from.</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int move(OPTree& other);
		/// <summary>
		/// Multiply this tree with 'node'.
		/// </summary>
		/// <param name="node">nodes to be multiplied</param>
		/// <returns>a new tree store the result</returns>
		OPTree mul_TO(const pNode& node);
		/// <summary>
		/// Multiply this tree with 'seq'.
		/// </summary>
		/// <param name="seq"></param>
		/// <param name="coef"></param>
		/// <returns></returns>
		OPTree mul_TO(const LabelSeq& seq, Complex coef);
		/// <summary>
		/// Multiply this tree with tree 'other'.
		/// </summary>
		/// <param name="other">tree to be multipied</param>
		/// <returns>a new tree store the result</returns>
		OPTree mul_TT(const OPTree& other);
		/// <summary>
		/// Multiply this tree with 'node'.
		/// Using connective multi operation
		/// </summary>
		/// <param name="node">nodes to be multiplied</param>
		/// <returns>a new tree store the result</returns>
		OPTree mul_Connect_TO(const pNode& node);
		/// <summary>
		/// Multiply this tree with tree 'other'.
		/// Using connective multi operation
		/// </summary>
		/// <param name="other">tree to be multipied</param>
		/// <returns>a new tree store the result</returns>
		OPTree mul_Connect_TT(const OPTree& other);
		/// <summary>
		/// remove a specific node
		/// </summary>
		/// <param name="nodeSeq">node's sequence to be removed</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int remove(const LabelSeq& nodeSeq);
		/// <summary>
		/// search for a specific node
		/// </summary>
		/// <param name="nodeSeq">the node's sequence</param>
		/// <returns>if succeed, return the pointer to the node; if not existed, return nullptr.</returns>
		pNode search(const LabelSeq& nodeSeq) const;
		/// <summary>
		/// Change the childSize and keep the topology of the tree at maximum
		/// </summary>
		/// <param name="childSize">new childSize</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int setChildSize(int childSize);
		/// <summary>
		/// shallow copy
		/// </summary>
		/// <param name="other">OPTree to be copied from</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int shallowCopy(const OPTree& other);
		/// <summary>
		/// Swap the content of two trees.
		/// </summary>
		/// <param name="other">OPTree to be swaped</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int swap(OPTree& other);
		/// <summary>
		/// Normalize the tree. Count the number of nodes and remove surplus nodes.
		/// </summary>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int normalize();
		OPTree& operator= (const OPTree& other);
		OPTree& operator= (OPTree&& other);

		/// <summary>
		/// get the iterator of this tree
		/// </summary>
		/// <returns>the iterator of this tree</returns>
		OPTree_Iterator getIterator(void) const;
		int getChildSize() const;
		int getNodeCount() const;

		// position tree related functions
		
		/// <summary>
		/// create a new OPTree, accroding to PTree and operator
		/// </summary>
		/// <param name="posTree">position tree</param>
		/// <param name="seq">operator</param>
		/// <returns>a new OPTree</returns>
		static OPTree BuildFromPTree(PTree& posTree, const LabelSeq& seq);

	private:
		static int _SortMOArray(LabelSeq& arr);
		// Root of the tree
		std::shared_ptr<Node> root;
		// The size of children of each node
		int childSize;
		// The size of existing nodes
		int nodeCount;

		/// <summary>
		/// Delete 'node'. If 'node' has no child, 'node' would be remove. This would recursively be applied to its parent.
		/// Caution: This method did not adjust the nodeCount
		/// </summary>
		/// <param name="node">node to be deleted</param>
		/// <returns>if succeed to remove, return 0; else return non-zero.</returns>
		int deleteNode_r(pNode& node);

		static void _free_node(pNode p);

		// position tree related functions
		static int _BuildFromPTree(pNode posNode, int posCsize, LabelSeq seq, pNode opNode, int opCsize);
	};

	typedef OPTree PTree;
}

#endif