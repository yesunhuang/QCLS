/**
 * @ 文件名: cluster_expansion.h
 * @ 功能: 声明与CE有关的函数
 * @ 作者: AyajiLin & YesunHuang
 * @ 邮箱: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ 日期: 06/24/2020
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
		/// 对应论文中的step1,2,3
		/// </summary>
		/// <param name="nowi">nowi</param>
		/// <param name="N">N</param>
		/// <param name="prevIndex">下一个用于buf中的索引号</param>
		/// <param name="alCount">已经用过的数目</param>
		/// <param name="buf">缓存区</param>
		/// <param name="outTree">结果</param>
		/// <returns>成功时,返回值为0; 否则,返回值为非0.</returns>
		static int _DT(int nowi, int N, int prevIndex, int alCount, Label* buf, OPTree& outTree);
		/// <summary>
		/// 将B树转为CE中的B树
		/// </summary>
		/// <param name="node"></param>
		/// <param name="tree"></param>
		/// <returns></returns>
		static int _BTToCEBT(pNode& node, OPTree& tree);
		/// <summary>
		/// 将每个结点的value取反
		/// </summary>
		/// <param name="node"></param>
		/// <param name="sth">nothing</param>
		static void _NegateNode(pNode node, void* sth);
	};
}


#endif
