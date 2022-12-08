
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Cell.h"

#include "Maruyama/Utility/Utility.h"

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


		//--------------------------------------------------------------------------------------
		///	デバッグ
		//--------------------------------------------------------------------------------------

		void Cell::OnDebugDraw() {
			if (m_debugDrawObject.expired()) {
				CreateDebugDrawObject();
				return;
			}

			auto transform = m_debugDrawObject.lock()->GetComponent<Transform>();
			transform->SetPosition(GetPosition());
		}

		void Cell::CreateDebugDrawObject() {
			auto stage = maru::Utility::GetStage();
			if (!stage) {
				return;
			}

			auto drawObject = stage->Instantiate<GameObject>(GetPosition(), Quat::Identity());
			m_debugDrawObject = drawObject;
		}

		void Cell::SetDebugDrawActive(const bool isDraw) {
			if (auto debugDraw = m_debugDrawObject.lock()) {
				debugDraw->SetDrawActive(isDraw);
			}
		}

	}
}