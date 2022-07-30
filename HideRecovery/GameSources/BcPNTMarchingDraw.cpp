/*!
@file BcPNTMarchingDraw.cpp
@brief BcPNTMarchingDraw�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "BcPNTMarchingDraw.h"

#include "MaruUtility.h"
#include "MetaballChildrenRender.h"

namespace basecross {

	constexpr int BaseTriTable[256][16] =
	{
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
			{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
			{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
			{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
			{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
			{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
			{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
			{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
			{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
			{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
			{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
			{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
			{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
			{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
			{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
			{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
			{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
			{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
			{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
			{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
			{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
			{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
			{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
			{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
			{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
			{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
			{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
			{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
			{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
			{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
			{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
			{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
			{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
			{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
			{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
			{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
			{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
			{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
			{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
			{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
			{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
			{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
			{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
			{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
			{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
			{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
			{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
			{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
			{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
			{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
			{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
			{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
			{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
			{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
			{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
			{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
			{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
			{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
			{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
			{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
			{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
			{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},

			{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
			{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
			{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
			{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
			{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
			{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
			{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
			{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
			{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
			{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
			{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
			{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
			{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
			{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
			{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
			{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
			{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
			{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
			{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
			{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
			{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
			{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
			{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
			{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
			{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
			{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
			{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
			{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
			{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
			{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
			{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
			{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
			{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
			{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
			{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
			{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
			{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
			{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
			{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
			{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
			{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
			{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
			{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},

			{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
			{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
			{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
			{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
			{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
			{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
			{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
			{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
			{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
			{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
			{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
			{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
			{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
			{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
			{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
			{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
			{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
			{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
			{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
			{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
			{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
			{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
			{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
			{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
			{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
			{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
			{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
			{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
			{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
			{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
			{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
			{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
			{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
			{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
			{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
			{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
			{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
			{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
			{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
			{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
			{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
			{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
			{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
			{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
			{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
			{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
			{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
			{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
			{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
			{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
			{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
			{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
			{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
			{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
			{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
			{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
			{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
			{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
			{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
			{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
			{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
			{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
			{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
			{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
			{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
			{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
			{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
			{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
			{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
			{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
			{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
			{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
			{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
			{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
			{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
	};

	static uint8_t TriTable[256][16] = {};

	using namespace Metaball;

	//�R���X�^���g�o�b�t�@�̎���
	IMPLEMENT_DX11_CONSTANT_BUFFER(MarchingBuffer)
	IMPLEMENT_DX11_CONSTANT_BUFFER(TriTableBuffer)

	namespace MarchingCube {

		//�R���X�^���g�o�b�t�@�ɓn�������p�����[�^�̂݋L�q����
		struct BcPNTMarchingDraw::Impl : public DrawObjectBase {
			//���_�ύX����ꍇ�̃��b�V���i�I���W�i���j
			shared_ptr<MeshResource> m_OriginalMeshResource;
			//�I���W�i�����b�V�����g�����ǂ���
			bool m_UseOriginalMeshResource;
			//�e�N�X�`�����\�[�X
			weak_ptr<TextureResource> m_TextureResource;
			//NormalMap�e�N�X�`��
			weak_ptr<TextureResource> m_NormalMapTextureResource;
			vector<weak_ptr<TextureResource>> m_NormalMapTextureResourceVec;
			//�G�~�b�V�u�F
			Col4 m_Emissive;
			//�f�t���[�Y�F
			Col4 m_Diffuse;
			//�����x
			float m_Alpha;
			//�X�y�L�����[�ƃp���[�F
			Col4 m_SpecularColorAndPower;
			//���C�e�B���O���邩�ǂ���
			bool m_LightingEnabled;
			//�s�N�Z�����C�e�B���O�����邩�ǂ���
			bool m_PreferPerPixelLighting;
			//�@���Ƀo�C�A�X�������邩�ǂ���
			bool m_BiasedNormals;
			//�A���r�G���g
			Col4 m_AmbientLightColor;
			//���C�g��MAX
			static const int MaxDirectionalLights = 3;
			//���C�g���L�����ǂ���
			bool m_LightEnabled[MaxDirectionalLights];
			//���C�g�̌���
			Vec3 m_LightDirection[MaxDirectionalLights];
			//���C�g�̃f�t���[�Y�F
			Col4 m_LightDiffuseColor[MaxDirectionalLights];
			//���C�g�̃X�y�L�����[�F
			Col4 m_LightSpecularColor[MaxDirectionalLights];
			//�t�H�O���L�����ǂ���
			bool m_FogEnabled;
			//�t�H�O�̊J�n�ʒu
			float m_FogStart;
			//�t�H�O�̏I���ʒu
			float m_FogEnd;
			//�t�H�O�F
			Col4 m_FogColor;
			//�t�H�O�x�N�g��
			Vec3 m_FogVector;
			//�e�𓊉e���邩�ǂ���
			bool m_OwnShadowActive;
			//���f���ɓ����Ă���Diffuse���g�����ǂ���
			bool m_ModelDiffusePriority;
			//���f���ɓ����Ă���Emissive���g�����ǂ���
			bool m_ModelEmissivePriority;
			//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
			bool m_ModelTextureEnabled;

			BcPNTMarchingDraw::Impl() :
				m_UseOriginalMeshResource(false),
				m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
				m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
				m_Alpha(1.0f),
				m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
				m_LightingEnabled(false),
				m_PreferPerPixelLighting(false),
				m_BiasedNormals(false),
				m_AmbientLightColor(0, 0, 0, 0),
				m_FogEnabled(false),
				m_FogStart(-25.0f),
				m_FogEnd(-40.0f),
				m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
				m_FogVector(0.0, 0.0, 1.0f),
				m_OwnShadowActive(false),
				m_ModelDiffusePriority(true),
				m_ModelEmissivePriority(true),
				m_ModelTextureEnabled(true)
			{
				static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
				static const bsm::Vec3 defaultLightDirection = { 0, -1, 0 };
				m_SpecularColorAndPower = defaultSpecular;
				for (int i = 0; i < MaxDirectionalLights; i++)
				{
					m_LightDirection[i] = defaultLightDirection;
					m_LightDiffuseColor[i] = g_XMZero;
					m_LightSpecularColor[i] = g_XMZero;
				}
			}
		};

		BcPNTMarchingDraw::BcPNTMarchingDraw(const std::shared_ptr<GameObject>& objPtr)
			:BcBaseDraw(objPtr), pImpl(new Impl())
		{}

		void BcPNTMarchingDraw::OnCreate() {
			SetLightingEnabled(true);
			//�}���`���C�g�̐ݒ�
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, true);
			}

			for (int i = 0; i < std::size(BaseTriTable); i++) {
				for (int j = 0; j < std::size(BaseTriTable[0]); j++) {
					TriTable[i][j] = static_cast<char>(BaseTriTable[i][j]);
				}
			}

			ComPtr<ID3D11ShaderResourceView> shaderResourceView;
			//�e�N�X�`���쐬
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			image.Release();

			memset(&metadata, 0, sizeof(TexMetadata));
			metadata.width = 256;
			metadata.height = 16;
			metadata.depth = 1;
			metadata.mipLevels = 1;
			metadata.dimension = TEX_DIMENSION_TEXTURE2D;
			metadata.arraySize = 1;
			//metadata.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			metadata.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//metadata.format = DXGI_FORMAT_R16_UINT;
			//metadata.format = DXGI_FORMAT_R32_UINT;
			metadata.SetAlphaMode(TEX_ALPHA_MODE_UNKNOWN);

			HRESULT hr = image.Initialize2D(metadata.format, metadata.width, metadata.height, metadata.arraySize, metadata.mipLevels);
			auto img = image.GetImage(0, 0, 0);
			auto& pixels = img->pixels;

			const int NumLoop = static_cast<int>(image.GetPixelsSize());
			int index = 0;
			for (int i = 0; i < NumLoop; i += 4) {
				auto n = *((&BaseTriTable[0][0]) + index++);
				pixels[i] = n;
				auto x = pixels[i];
				pixels[i + 1] = 0;
				if (n == -1) {
					pixels[i + 1] = -1;
					char y = pixels[i + 1];
					char yy = 1;
				}
				else {
					pixels[i + 1] = 0;
				}
				pixels[i + 2] = 0;
				pixels[i + 3] = 0;
			}

			//for (int i = 0; i < NumLoop; i++) {
			//	auto n = *((&BaseTriTable[0][0]) + i);
			//	auto ss = sizeof(uint16_t);
			//	int index = i * sizeof(uint16_t);

			//	auto nStr = to_string(1 * 255);
			//	//string nStr(tempStr.rbegin(), tempStr.rend());
			//	auto nstrcstrPtr = nStr.c_str();
			//	auto str1 = nstrcstrPtr[0];
			//	auto str2 = nstrcstrPtr[1];
			//	auto str3 = nstrcstrPtr[2];

			//	//pixels[index] = nStr[0];
			//	//pixels[index + 1] = nStr[1];
			//	pixels[index] = *nStr.c_str();
		
			//	auto x = pixels[index];
			//	auto x2 = pixels[index + 1];
			//	auto s = 0;
			//}

			//vector<uint8_t> sss;
			//vector<uint16_t> result;
			//for (int i = 0; i < NumLoop; i++) {
			//	auto index = i * sizeof(uint16_t);
			//	//std::atoi(&);
			//	string str;
			//	str.push_back(pixels[index]);
			//	str.push_back(pixels[index + 1]);
			//	auto re = std::atoi(str.c_str());
			//	result.push_back(re);
			//}

			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_shaderResourceView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// �摜����V�F�[�_���\�[�XView�̍쐬
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
					L"if( FAILED( CreateShaderResourceView() ) )",
					L"Texture::Impl::Impl()"
				);
				return hr;
			});
		}

		void BcPNTMarchingDraw::OnDraw() {

			if (GetGameObject()->GetAlphaActive()) {
				if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
					SetBlendState(BlendState::AlphaBlend);
				}
				SetRasterizerState(RasterizerState::DoubleDraw);
			}

			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetMeshResource();
			if (PtrMeshResource) {
				DrawStatic2(PtrMeshResource->GetMashData());

				//�V�F�[�_�̐ݒ�
				if (IsPerPixelLighting()) {
					//�s�N�Z�����C�e�B���O
					//DrawStatic<VSPNTPoint, PSPNTPoint>(PtrMeshResource->GetMashData());
				}
				else {
					//���_���C�e�B���O
					//DrawStatic<VSPNTPoint, PSPNTPoint>(PtrMeshResource->GetMashData());
				}
			}

			
		}

		void BcPNTMarchingDraw::DrawStatic2(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(MarchingCubeVS::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(MarchingCubeVS::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(MarchingCubePS::GetPtr()->GetShader(), nullptr, 0);

			//���C�g��ݒ�
			SetLightingParamaters(); //�K�v�Ȃ���������Ȃ�
			//�ʏ���
			MarchingConstants pointCb;  //�g�p����o�b�t�@�ɂ���ĕς��
			TriTableConstants triTableCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetMarchingConstants(pointCb, data);  //���ꂻ����������������
			SetTriTableConstants(triTableCb, data);
			//�e�N�X�`��
			auto shTex = GetTextureResource();
			if (shTex) {
				//�e�N�X�`��������
				pointCb.activeFlg.y = 1;
			}
			else {
				//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
				if (shTex = data.m_TextureResource.lock()) {
					//�e�N�X�`��������
					pointCb.activeFlg.y = 1;
				}
				else {
					pointCb.activeFlg.y = 0;
				}
			}
			//�R���X�^���g�o�b�t�@�̍X�V
			//�o�b�t�@�̎O�m�͕ς��
			pD3D11DeviceContext->UpdateSubresource(MarchingBuffer::GetPtr()->GetBuffer(), 0, nullptr, &pointCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = MarchingBuffer::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			//TriTable�R���X�^���g�o�b�t�@
			pD3D11DeviceContext->UpdateSubresource(TriTableBuffer::GetPtr()->GetBuffer(), 0, nullptr, &triTableCb, 0, 0);
			pConstantBuffer = TriTableBuffer::GetPtr()->GetBuffer();
			pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(1, 1, &pConstantBuffer);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//�`����@�̃Z�b�g
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�����_�����O�X�e�[�g�̐ݒ�
			//�u�����h�X�e�[�g
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//�f�v�X�X�e���V���X�e�[�g
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//�e�N�X�`���ƃT���v���[
			pD3D11DeviceContext->VSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());
			pD3D11DeviceContext->PSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());

			//UnorderedAccessView
			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			//�f�o�C�X�̎擾
			//auto Dev = App::GetApp()->GetDeviceResources();
			//ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			//ComPtr<ID3D11UnorderedAccessView> access;
			//ComPtr<ID3D11Resource> resource;
			//ComPtr<D3D11_UNORDERED_ACCESS_VIEW_DESC> desc;
			//
			//pDx11Device->CreateUnorderedAccessView(resource.Get(), desc.Get(), access.GetAddressOf());
			//pD3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, access.GetAddressOf(), 0);
			
			//�T���v���[��ݒ�
			//SetSamplerState(SamplerState::ComparisonLinear);
			RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			if (shTex) {
				//pD3D11DeviceContext->VSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());
				//pD3D11DeviceContext->PSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());
				////pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				////�T���v���[��ݒ�
				//RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
				//�@���}�b�v
				if (GetNormalMapTextureResource()) {
					if (IsOwnShadowActive()) {
						//�Q�Ԗڂɓ����
						pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//�T���v���[��ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
					}
					else {
						//1�Ԗڂɓ����
						pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//�T���v���[��ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
					}
				}
			}
			else {
				//�V�F�[�_�[���\�[�X���N���A
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//�T���v���[���N���A
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}
			//�e�ƃT���v���[
			if (IsOwnShadowActive()) {
				//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//�V���h�E�}�b�v�T���v���[
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			//���X�^���C�U�X�e�[�g�ƕ`��
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//���������p
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}

			//��n��
			Dev->InitializeStates();
		}


		//�����������Ő��������R���X�^���g�o�b�t�@�p�̃R���X�g���N�g�ɕς���B
		void BcPNTMarchingDraw::SetMarchingConstants(MarchingConstants& pointCb, const MeshPrimData& data) {
			//�s��̒�`
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			//�J�����𓾂�
			auto CameraPtr = GetGameObject()->OnGetDrawCamera();
			//���C�g�𓾂�
			auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
			auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
			if (PtrMultiLight) {
				//�}���`���C�g������
				pointCb.activeFlg.x = 3;
			}
			else {
				//�����ł͂Ȃ�
				pointCb.activeFlg.x = 1;
			}
			auto StageLight = GetGameObject()->OnGetDrawLight();
			//���[���h�s��
			bsm::Mat4x4 world;
			if (data.m_UseMeshToTransformMatrix) {
				world = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
				world *= PtrTrans->GetWorldMatrix();
			}
			else {
				world = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
			}
			//�r���[�s��
			bsm::Mat4x4 view = CameraPtr->GetViewMatrix();
			//�ˉe�s��
			bsm::Mat4x4 projection = CameraPtr->GetProjMatrix();

			//�s��̐ݒ�
			auto worldView = world * view;
			pointCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

			pointCb.viewProj = XMMatrixTranspose(XMMatrixMultiply(view, projection));

			//�t�H�O�̐ݒ�
			if (IsFogEnabled())
			{
				auto start = GetFogStart();
				auto end = GetFogEnd();
				if (start == end)
				{
					// Degenerate case: force everything to 100% fogged if start and end are the same.
					static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
					pointCb.fogVector = fullyFogged;
				}
				else
				{
					XMMATRIX worldViewTrans = worldView;
					// _13, _23, _33, _43
					XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
						XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
					XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
					pointCb.fogVector = (worldViewZ + wOffset) / (start - end);
				}
				pointCb.fogColor = GetFogColor();
			}
			else
			{
				pointCb.fogVector = g_XMZero;
				pointCb.fogColor = g_XMZero;

			}

			//�����ł��B
			for (int i = 0; i < m_renders.size(); i++) {
				if (!m_renders[i]) {
					continue;
				}

				pointCb.spheres[i] = m_renders[i]->GetSphereData();
				pointCb.colors[i] = m_renders[i]->GetColor();
			}

			pointCb.useNumSpheres = static_cast<int>(m_renders.size());
			//pointCb.useNumSpheres = static_cast<int>(2.0f);

			//���C�g�̐ݒ�
			if (IsLightingEnabled())
			{
				//���s���C�g
				if (pointCb.activeFlg.x == 1) {
					pointCb.lightDirection[0] = GetLightDirection(0);
					pointCb.lightDiffuseColor[0] = GetLightDiffuseColor(0);
					pointCb.lightSpecularColor[0] = GetLightSpecularColor(0);
				}
				else {
					for (int i = 0; i < GetMaxDirectionalLights(); i++) {
						if (IsLightEnabled(i)) {
							pointCb.lightDirection[i] = GetLightDirection(i);
							pointCb.lightDiffuseColor[i] = GetLightDiffuseColor(i);
							pointCb.lightSpecularColor[i] = GetLightSpecularColor(i);
						}
					}
				}

				//���[���h�n�̏�������B
				pointCb.world = XMMatrixTranspose(world);

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);

				pointCb.worldInverseTranspose[0] = worldInverse.r[0];
				pointCb.worldInverseTranspose[1] = worldInverse.r[1];
				pointCb.worldInverseTranspose[2] = worldInverse.r[2];

				XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);

				pointCb.eyePosition = viewInverse.r[3];
			}
			XMVECTOR diffuse = GetDiffuse();
			XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
			XMVECTOR emissiveColor = GetEmissive();
			XMVECTOR ambientLightColor = GetAmbientLightColor();
			if (IsLightingEnabled())
			{
				// Merge emissive and ambient light contributions.
				pointCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
				pointCb.specularColorAndPower = GetSpecularColorAndPower();
			}
			else
			{
				pointCb.activeFlg.x = 0;
				// Merge diffuse and emissive light contributions.
				diffuse += emissiveColor;
				pointCb.specularColorAndPower = GetSpecularColorAndPower();
			}
			// xyz = diffuse * alpha, w = alpha.
			pointCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

			//�e�p
			if (GetOwnShadowActive()) {
				Vec3 CalcLightDir(StageLight.m_Directional * -1.0);
				Vec3 LightAt(CameraPtr->GetAt());
				Vec3 LightEye(CalcLightDir);
				LightEye *= Shadowmap::GetLightHeight();
				LightEye = LightAt + LightEye;
				Vec4 LightEye4(LightEye, 1.0f);
				LightEye4.w = 1.0f;
				pointCb.lightPos = LightEye4;
				Vec4 eyePos4(CameraPtr->GetEye(), 1.0f);
				eyePos4.w = 1.0f;
				pointCb.eyePos = eyePos4;
				Mat4x4 LightView, LightProj;
				//���C�g�̃r���[�Ǝˉe���v�Z
				LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
				LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
					Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
				pointCb.lightView = bsm::transpose(LightView);
				pointCb.lightProjection = bsm::transpose(LightProj);
			}
		}

		void BcPNTMarchingDraw::SetTriTableConstants(TriTableConstants& triTableCb, const MeshPrimData& data) {
			for (int i = 0; i < std::size(BaseTriTable); i++) {
				for (int j = 0; j < std::size(BaseTriTable[0]); j++) {
					triTableCb.triTable[i][j] = BaseTriTable[i][j];
				}
			}

		}


		void BcPNTMarchingDraw::SetRenders(const vector<ex_weak_ptr<ChildrenRender>>& renders) {
			m_renders = renders;
		}

	}
}