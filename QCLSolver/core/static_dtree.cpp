/**
 * @ 文件名: static_dtree.c
 * @ 功能: delta树的静态存储
 * @ 作者: AyajiLin & YesunHuang
 * @ 邮箱: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ 日期: 06/23/2020
 *
 */
#include "static_dtree.h"

#include <cmath>

namespace ayaji {
	OPTree* StaticDTree::g_StanamicDTrees[STANAMIC_DTREE_SIZE + 1] = { NULL };

	int StaticDTree::g_StaticDTreesIndexList[STATIC_DTREE_SIZE + 1] = { 0,2,10,42,160,616 };
	/* 添加前五棵树的静态数组 */
	Label StaticDTree::g_StaticDTreesList[] =
	{
		/* DeltaTree(1) */
		1, 255,
		/* DeltaTree(2) */
		1, 0, 2, 255, 255, 2, 255, 255,
		/* DeltaTree(3) */
		1, 0, 2, 0, 3, 255, 255, 3, 255, 255, 255, 2, 3, 255, 255, 255,
		2, 0, 1, 3, 255, 255, 255, 255, 3, 0, 1, 2, 255, 255, 255, 255,
		/* DeltaTree(4) */
		1, 0, 2, 0, 3, 0, 4, 255, 255, 4, 255, 255, 255, 3, 4, 255, 255, 255,
		3, 0, 2, 4, 255, 255, 255, 255, 4, 0, 2, 3, 255, 255, 255, 255, 255,
		2, 0, 3, 4, 255, 255, 255, 3, 4, 255, 255, 255, 3, 0, 2, 4, 255, 255,
		255, 255, 4, 0, 2, 3, 255, 255, 255, 255, 255, 2, 0, 1, 3, 4, 255, 255,
		255, 3, 0, 1, 4, 255, 255, 255, 255, 4, 0, 1, 3, 255, 255, 255, 255, 255,
		255, 3, 0, 1, 2, 4, 255, 255, 255, 4, 0, 1, 2, 255, 255, 255, 255, 255,
		255, 4, 0, 1, 2, 3, 255, 255, 255, 255, 255,
		/* DeltaTree(5) */
		1, 0, 2, 0, 3, 0, 4, 0, 5, 255, 255, 5, 255, 255, 255, 4, 5, 255, 255,
		255, 4, 0, 3, 5, 255, 255, 255, 255, 5, 0, 3, 4, 255, 255, 255, 255, 255,
		3, 0, 4, 5, 255, 255, 255, 4, 5, 255, 255, 255, 4, 0, 3, 5, 255, 255, 255,
		255, 5, 0, 3, 4, 255, 255, 255, 255, 255, 3, 0, 2, 4, 5, 255, 255, 255, 4,
		0, 2, 5, 255, 255, 255, 255, 5, 0, 2, 4, 255, 255, 255, 255, 255, 255, 4, 0,
		2, 3, 5, 255, 255, 255, 5, 0, 2, 3, 255, 255, 255, 255, 255, 255, 5, 0, 2, 3,
		4, 255, 255, 255, 255, 255, 255, 2, 0, 3, 4, 5, 255, 255, 255, 255, 3, 4, 5,
		255, 255, 255, 255, 3, 0, 2, 4, 5, 255, 255, 255, 255, 255, 4, 0, 2, 3, 5,
		255, 255, 255, 255, 255, 5, 0, 2, 3, 4, 255, 255, 255, 255, 255, 255, 2, 0,
		1, 3, 0, 4, 5, 255, 255, 255, 4, 5, 255, 255, 255, 4, 0, 3, 5, 255, 255, 255,
		255, 5, 0, 3, 4, 255, 255, 255, 255, 255, 3, 0, 1, 4, 5, 255, 255, 255, 4, 0,
		1, 5, 255, 255, 255, 255, 5, 0, 1, 4, 255, 255, 255, 255, 255, 255, 4, 0, 1,
		3, 5, 255, 255, 255, 5, 0, 1, 3, 255, 255, 255, 255, 255, 255, 5, 0, 1, 3, 4,
		255, 255, 255, 255, 255, 255, 3, 0, 1, 4, 5, 255, 255, 255, 255, 255, 4, 0, 1,
		3, 5, 255, 255, 255, 255, 255, 5, 0, 1, 3, 4, 255, 255, 255, 255, 255, 255, 3,
		0, 1, 2, 0, 4, 5, 255, 255, 255, 4, 5, 255, 255, 255, 4, 0, 2, 5, 255, 255, 255,
		255, 5, 0, 2, 4, 255, 255, 255, 255, 255, 4, 0, 1, 2, 5, 255, 255, 255, 5, 0, 1,
		2, 255, 255, 255, 255, 255, 255, 5, 0, 1, 2, 4, 255, 255, 255, 255, 255, 255, 4,
		0, 1, 2, 5, 255, 255, 255, 255, 255, 5, 0, 1, 2, 4, 255, 255, 255, 255, 255, 255,
		4, 0, 1, 2, 0, 3, 5, 255, 255, 255, 3, 5, 255, 255, 255, 3, 0, 2, 5, 255, 255, 255,
		255, 5, 0, 2, 3, 255, 255, 255, 255, 255, 5, 0, 1, 2, 3, 255, 255, 255, 255, 255,
		255, 5, 0, 1, 2, 3, 255, 255, 255, 255, 255, 255, 5, 0, 1, 2, 0, 3, 4, 255, 255,
		255, 3, 4, 255, 255, 255, 3, 0, 2, 4, 255, 255, 255, 255, 4, 0, 2, 3, 255, 255,
		255, 255, 255, 255, 255
	};
	int StaticDTree::g_StaticDTreesValueRealIndexList[STATIC_DTREE_SIZE + 1] = { 0,1,5,21,80,308 };
	int StaticDTree::g_StaticDTreesValueRealList[] =
	{
		/* DeltaTree(1) */
		1,
		/* DeltaTree(2) */
		0, 0, -1, 1,
		/* DeltaTree(3) */
		0, 0, 0, 0, 2, -1, 0, 1, 0, 0, 0, -1, 0, 0, 0, -1,
		/* DeltaTree(4) */
		0, 0, 0, 0, 0, 0, -6, 2, 0, -1, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0,
		-1, 0, 1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 2,
		0, 0, 0, 2, 0, 0, 0, 0, -1, 0, 0, 0, 2, 0, 0, 0, 0, -1,
		/* DeltaTree(5) */
		0, 0, 0, 0, 0, 0, 0, 0, 24, -6, 0, 2, 0, 0, 0, -6, 0, 0, 0, -6, 0, 0, 0,
		2, 0, -1, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, -6, 0, 0,
		0, -6, 0, 0, 0, 0, 2, 0, 0, 0, -6, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1, 0,
		0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0,
		0, 2, 0, -1, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, -6, 0,
		0, 0, -6, 0, 0, 0, 0, 2, 0, 0, 0, -6, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1,
		0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 2, 0, -1, 0, 0, 0,
		2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, -6, 0, 0, 0, 0, 2, 0, 0, 0,
		0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 2, 0, -1, 0, 0, 0, 2, 0, 0,
		0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 2, 0, -1, 0,
		0, 0, 2, 0, 0, 0, 2
	};

	OPTree StaticDTree::CreateStaticDTree(int n) {
		if (n <= 0 || n > STATIC_DTREE_SIZE)
			return NULL;
		int startIndex = g_StaticDTreesIndexList[n - 1];
		int endIndex = g_StaticDTreesIndexList[n];
		int startValueIndex = g_StaticDTreesValueRealIndexList[n - 1];
		int endValueIndex = g_StaticDTreesValueRealIndexList[n];
		Label maxLabel = 0;
		for (int i = startIndex; i < endIndex; ++i) {
			if (g_StaticDTreesList[i] != 255) {
				maxLabel = std::max(maxLabel, g_StaticDTreesList[i]);
			}
		}
		OPTree ret;
		ret.setChildSize(maxLabel);

		pNode nowNode = &(*ret.root);
		int listIndex = startIndex;
		int valueListIndex = startValueIndex;
		while (listIndex < endIndex && valueListIndex < endValueIndex) {
			if (g_StaticDTreesList[listIndex] == 255) {
				nowNode = nowNode->parent;
				++listIndex;
			}
			else {
				Label nextLabel = g_StaticDTreesList[listIndex];
				if (nowNode->children[nextLabel] == nullptr) {
					nowNode->children[nextLabel] = Node::createNode(ret.childSize, 0, Complex(), nowNode);
				}
				nowNode = nowNode->children[nextLabel];
				nowNode->label = g_StaticDTreesList[listIndex];
				nowNode->value.setReal(g_StaticDTreesValueRealList[valueListIndex]);
				if (!nowNode->value.isZero())
					ret.nodeCount += 1;
				++listIndex;
				++valueListIndex;
			}
		}


		return ret;
	}
}
