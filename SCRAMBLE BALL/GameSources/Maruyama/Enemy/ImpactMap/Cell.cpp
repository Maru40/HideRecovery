
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	セルパラメータ
		//--------------------------------------------------------------------------------------

		Cell_Parametor::Cell_Parametor() :
			Cell_Parametor(Rect(Vec3(0.0f), 2.0f, 2.0f))
		{}

		Cell_Parametor::Cell_Parametor(const Rect& rect) :
			rect(rect)
		{}

		//--------------------------------------------------------------------------------------
		///	セル
		//--------------------------------------------------------------------------------------

		Cell::Cell(const Parametor& parametor) :
			m_param(parametor)
		{}

	}
}