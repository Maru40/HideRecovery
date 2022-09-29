
/*!
@file NodeBase.cpp
@brief NodeBase�̃N���X����
�S���F�ێR�T��
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