
/*!
@file GraphNodeBase.cpp
@brief GraphNodeBaseのクラス実体
担当：丸山裕喜
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