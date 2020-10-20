/**
 * @ 文件名: static_dtree.h
 * @ 功能: delta树的静态(动态)存储
 * @ 作者: AyajiLin & YesunHuang
 * @ 邮箱: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ 日期: 06/23/2020
 *
 */
#pragma once
#ifndef _STATIC_DTREE_H_
#define _STATIC_DTREE_H_

#include "operator_tree.h"

/* 静态存储的D树个数 */
#define STATIC_DTREE_SIZE 5
/* 允许动态存储的D树最大个数 */
#define STANAMIC_DTREE_SIZE 16

namespace ayaji {
	class OPTree;

	class StaticDTree {
	public:
		static OPTree CreateStaticDTree(int n);
		static OPTree* g_StanamicDTrees[STANAMIC_DTREE_SIZE + 1];

		static int g_StaticDTreesIndexList[STATIC_DTREE_SIZE + 1];
		static Label g_StaticDTreesList[];
		static int g_StaticDTreesValueRealIndexList[STATIC_DTREE_SIZE + 1];
		static int g_StaticDTreesValueRealList[];
	};
}

#endif // !_STATIC_DTREE_H_


