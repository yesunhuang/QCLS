#pragma once

#ifndef _COMMON_H_
#define _COMMON_H_

#include "node.h"

namespace ayaji {
	class OPTree;
	class ClusterExp;
	class DeriveData;


	class Common {
	public:
		static const int MAX_OPERATOR_LENGTH = 128;
		static LabelSeq mul(const LabelSeq& label1, const LabelSeq& label2);
		static LabelSeq mul_Connect(const LabelSeq& label1, const LabelSeq& label2);
		// return the maximum of the label value
		static Label maxLabel(const LabelSeq& seq);

		/// <summary>
		/// 共轭一个single算符
		/// </summary>
		/// <param name="seq"></param>
		/// <returns></returns>
		static LabelSeq barSeq(const LabelSeq& seq);

		static int _GetNextCPIndexFromOPArray(const LabelSeq& seq, int now);
		static int _CompareOPArrayBuffer(const LabelSeq& seq1, int s1, const LabelSeq& seq2, int s2);
	};
}

#endif // !_COMMON_H_