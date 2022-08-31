
/*!
@file GraphNodeBase.cpp
@brief GraphNodeBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "GraphNodeBase.h"

namespace basecross {

	GraphNodeBase::GraphNodeBase()
		: GraphNodeBase(0)
	{}

	GraphNodeBase::GraphNodeBase(const int& index)
		: m_index(index), m_isActive(true)
	{}

}