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
	void deb_printTree(const OPTree& tree);
	void deb_printTree_(pNode& node, LabelSeq& seqBuf, int cSize);

	void deb_printData(DeriveData& data);
}


#endif // !_DEB_H_
#endif