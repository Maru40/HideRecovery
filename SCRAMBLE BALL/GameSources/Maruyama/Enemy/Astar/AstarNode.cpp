
/*!
@file AstarNode.cpp
@brief AstarNodeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarNode.h"

namespace basecross {

	namespace maru {

		AstarNode::AstarNode():
			AstarNode(0)
		{}

		AstarNode::AstarNode(const int index) :
			NodeBase(index)
		{}

		

	}

}