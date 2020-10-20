/**
 * @ �ļ���: cluster_expansion.h
 * @ ����: ������CE�йصĺ���
 * @ ����: AyajiLin & YesunHuang
 * @ ����: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ ����: 06/24/2020
 *
 */
#pragma once

#ifndef _CLUSTER_EXPANSION_H_
#define _CLUSTER_EXPANSION_H_

#include "operator_tree.h"
#include "static_dtree.h"

namespace ayaji {
	class OPTree;

	class ClusterExp {
	public:
		static const int MAX_LABEL_BUFFER_SIZE = 128;
		/// <summary>
		/// return deltatree of size 'N'
		/// </summary>
		/// <param name="N">deltatree size</param>
		/// <returns>deltatree</returns>
		static OPTree DeltaTree(int N);
		/// <summary>
		/// cluster expansion
		/// </summary>
		/// <param name="seq">operator label sequence</param>
		/// <returns>operator tree</returns>
		static OPTree ClusterExpansion(const LabelSeq& seq);
		/// <summary>
		/// transform delta tree to B tree
		/// </summary>
		/// <param name="tree">tree</param>
		/// <param name="seq">seq</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		static int DTToBT(OPTree& tree, LabelSeq seq);
		/// <summary>
		/// transform B tree to B tree in CE
		/// </summary>
		/// <param name="tree">tree</param>
		/// <returns>if succeed, return 0; else return non-zero.</returns>
		static int BTToCEBT(OPTree& tree);
		/// <summary>
		/// ��Ӧ�����е�step1,2,3
		/// </summary>
		/// <param name="nowi">nowi</param>
		/// <param name="N">N</param>
		/// <param name="prevIndex">��һ������buf�е�������</param>
		/// <param name="alCount">�Ѿ��ù�����Ŀ</param>
		/// <param name="buf">������</param>
		/// <param name="outTree">���</param>
		/// <returns>�ɹ�ʱ,����ֵΪ0; ����,����ֵΪ��0.</returns>
		static int _DT(int nowi, int N, int prevIndex, int alCount, Label* buf, OPTree& outTree);
		/// <summary>
		/// ��B��תΪCE�е�B��
		/// </summary>
		/// <param name="node"></param>
		/// <param name="tree"></param>
		/// <returns></returns>
		static int _BTToCEBT(pNode& node, OPTree& tree);
		/// <summary>
		/// ��ÿ������valueȡ��
		/// </summary>
		/// <param name="node"></param>
		/// <param name="sth">nothing</param>
		static void _NegateNode(pNode node, void* sth);
	};
}


#endif
