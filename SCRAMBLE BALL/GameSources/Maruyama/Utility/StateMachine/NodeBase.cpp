
/*!
@file NodeBase.cpp
@brief NodeBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "NodeBase.h"

namespace basecross {

	namespace maru {

		NodeBase::NodeBase() :
			NodeBase(0)
		{}

		NodeBase::NodeBase(const int index) :
			m_isActive(true),
			m_index(index)
		{}

	}
}