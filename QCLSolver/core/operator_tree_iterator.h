/**
 * @ FileName: operator_tree_iterator.h
 * @ Function: declare the iterator of operator tree
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 06/21/2020
 *
 */

#pragma once
#ifndef _OPERATOR_TREE_ITERATOR_H_
#define _OPERATOR_TREE_ITERATOR_H_

#include "operator_tree.h"

/// <summary>
/// a class used to go through OPTree
/// </summary>
namespace ayaji {
	class Node;
	class OPTree;

	class OPTree_Iterator {
	public:
		OPTree_Iterator(const OPTree& tree);
		OPTree_Iterator(OPTree_Iterator&& other) noexcept;

		/// <summary>
		/// get the current pnode and move to the next non-zero node
		/// </summary>
		/// <returns>next pnode</returns>
		pNode next(void);
		/// <summary>
		/// TODO: remove this node, and move to the next non-zero node
		/// </summary>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		int remove(void);
		
		/// <summary>
		/// get the current node
		/// </summary>
		/// <returns>the current node</returns>
		pNode getNode(void);
		/// <summary>
		/// get the current node
		/// </summary>
		/// <returns>the current node</returns>
		pNode operator* (void);
		/// <summary>
		/// get the current node's label sequence
		/// </summary>
		/// <returns>the current node's label sequence</returns>
		LabelSeq getCurLabelSeq(void);

	private:
		const OPTree* tree;
		pNode cur;
		/// <summary>
		/// indicate whether is behind the last
		/// </summary>
		bool isTail;
		LabelSeq seqStack;
		std::vector<pNode> nodeStack;
	};
}


#endif