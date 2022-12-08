
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Cell.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/DebugClass/Component/RectDraw.h"

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


		//--------------------------------------------------------------------------------------
		///	�f�o�b�O
		//--------------------------------------------------------------------------------------

		void Cell::OnDebugDraw() {
			OnDebugDraw(maru::Utility::GetStage());
		}

		void Cell::OnDebugDraw(const std::shared_ptr<Stage>& stage) {
			if (m_debugDrawObject.expired()) {
				CreateDebugDrawObject(stage);
				return;
			}

			auto transform = m_debugDrawObject.lock()->GetComponent<Transform>();
			transform->SetPosition(GetPosition());
		}

		void Cell::CreateDebugDrawObject(const std::shared_ptr<Stage>& stage) {
			//�X�e�[�W�����݂��Ȃ�������A�������΂��B
			if (!stage) {
				return;
			}

			auto drawObject = stage->Instantiate<GameObject>(GetPosition(), Quat::Identity());
			drawObject->AddComponent<RectDraw>(m_param.rect);
			m_debugDrawObject = drawObject;
		}

		void Cell::SetDebugDrawActive(const bool isDraw) {
			if (auto debugDraw = m_debugDrawObject.lock()) {
				debugDraw->SetDrawActive(isDraw);
			}
		}

	}
}