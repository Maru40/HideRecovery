
/*!
@file Debug_DrawController.cpp
@brief Debug_DrawControllerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Debug_DrawController.h"

#include "Maruyama/DebugClass/Component/RectDraw.h"
#include "Maruyama/Utility/Utility.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// 表示用データ
		//--------------------------------------------------------------------------------------

		Debug_DrawData::Debug_DrawData() :
			Debug_DrawData(maru::Rect(Vec3(0.0f), 1.0f, 1.0f))
		{}

		Debug_DrawData::Debug_DrawData(const maru::Rect& rect) :
			rect(rect)
		{}

		//--------------------------------------------------------------------------------------
		/// デバッグ用のドローコントローラー
		//--------------------------------------------------------------------------------------

		Debug_DrawController::Debug_DrawController() :
			Debug_DrawController(Debug_DrawData())
		{}

		Debug_DrawController::Debug_DrawController(const Debug_DrawData& data) :
			m_data(data)
		{}

		void Debug_DrawController::OnDraw(const Vec3& position) {
			if (m_drawObject.expired()) {
				CreateDebugDrawObject(position);
				return;
			}

			auto transform = m_drawObject.lock()->GetComponent<Transform>();
			transform->SetPosition(position);
		}

		void Debug_DrawController::CreateDebugDrawObject(const Vec3& position) {
			auto stage = maru::Utility::GetStage();
			//ステージが存在しなかったら、処理を飛ばす。
			if (!stage) {
				return;
			}

			auto drawObject = stage->Instantiate<GameObject>(position, Quat::Identity());
			drawObject->AddComponent<RectDraw>(m_data.rect);
			m_drawObject = drawObject;
		}

		void Debug_DrawController::SetDebugDrawActive(const bool isDraw) {
			if (auto debugDraw = GetDrawObject()) {
				debugDraw->SetDrawActive(isDraw);
			}
		}

	}
}