#pragma once
#define DEB
#ifdef DEB
#ifndef _DEB_H_
#define _DEB_H_

#include <iostream>
#include <stdarg.h>

#include "operator_tree.h"
#include "derive_data.h"

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

	std::vector<std::pair<Complex, LabelSeq>> deb_Tree2Pair(const OPTree& tree);
	void deb_Tree2Pair_(pNode& node, LabelSeq& seqBuf, int cSize, std::vector<std::pair<Complex, LabelSeq>>& pairs);

	void deb_PrintData(DeriveData& data);
}


#endif // !_DEB_H_
#endif