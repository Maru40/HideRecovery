/*!
@file I_PriorityController.cpp
@brief I_PriorityController‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "I_PriorityController.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			PriorityControllerBase::PriorityControllerBase() :
				PriorityControllerBase(0.0f)
			{}

			PriorityControllerBase::PriorityControllerBase(const float priority) :
				m_priority(priority)
			{}

		}
	}
}