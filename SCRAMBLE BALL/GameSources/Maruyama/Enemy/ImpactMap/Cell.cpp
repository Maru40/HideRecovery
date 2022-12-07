
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�Z���p�����[�^
		//--------------------------------------------------------------------------------------

		Cell_Parametor::Cell_Parametor() :
			Cell_Parametor(Rect(Vec3(0.0f), 2.0f, 2.0f))
		{}

		Cell_Parametor::Cell_Parametor(const Rect& rect) :
			rect(rect)
		{}

		//--------------------------------------------------------------------------------------
		///	�Z��
		//--------------------------------------------------------------------------------------

		Cell::Cell(const Parametor& parametor) :
			m_param(parametor)
		{}

	}
}