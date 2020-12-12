#pragma once

#ifndef _DEB_H_
#define _DEB_H_

#include <iostream>
#include <stdarg.h>
#include <type_traits>

#include "operator_tree.h"
#include "common.h"

namespace ayaji {
	/// <summary>
	/// create LabelSeq for debugging
	/// </summary>
	/// <param name="size">label seq size</param>
	/// <param name="...">must be in 'int'</param>
	/// <returns>new label seq</returns>
	LabelSeq deb_generateLabelSeq(unsigned size, ...);
	/// <summary>
	/// print out the tree to stdout
	/// </summary>
	/// <param name="tree">tree to be printed</param>
	void deb_PrintTree(const OPTree& tree);
	void deb_PrintTree_(pNode& node, LabelSeq& seqBuf, int cSize);

	template <typename _Ty, typename = void>
	struct is_outputer : std::false_type {};
	template <typename _Ty>
	struct is_outputer<_Ty, decltype((void)(void (_Ty::*)(Complex, LabelSeq)) &_Ty::operator())> : std::true_type {};
	template<typename T>
	void deb_PrintTree_(pNode& node, LabelSeq& seqBuf, int cSize, T& outputer);
	template<typename T>
	void deb_PrintTree(const OPTree& tree, T& func) {
		static_assert(is_outputer<T>::value, "Debug outputer dont have () operator!\n");
		if (tree.root == nullptr) return;
		LabelSeq seqBuf;
		for (int i = 0; i <= tree.childSize; ++i) {
			pNode node;
			if ((node = tree.root->children[i]) != nullptr) {
				deb_PrintTree_(node, seqBuf, tree.childSize, func);
			}
		}
	}
	template<typename T>
	void deb_PrintTree_(pNode& node, LabelSeq& seqBuf, int cSize, T& outputer) {
		if (node == nullptr) return;
		seqBuf.push_back(node->label);
		if (!node->value.isZero()) {
			outputer(node->value, seqBuf);
		}
		for (int i = 0; i <= cSize; ++i) {
			pNode tnode;
			if ((tnode = node->children[i]) != nullptr) {
				deb_PrintTree_(tnode, seqBuf, cSize, outputer);
			}
		}
		seqBuf.pop_back();
	}

	std::vector<std::pair<Complex, LabelSeq>> deb_Tree2Pair(const OPTree& tree);
	void deb_Tree2Pair_(pNode& node, LabelSeq& seqBuf, int cSize, std::vector<std::pair<Complex, LabelSeq>>& pairs);

	

	void deb_PrintData(DeriveData& data);
}


#endif // !_DEB_H_