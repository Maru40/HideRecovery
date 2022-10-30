
/*!
@file HidePlaceOpener.cpp
@brief HidePlaceOpenerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlaceOpener.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/StageObject/HidePlace.h"

namespace basecross {

	HidePlaceOpener::HidePlaceOpener(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_range(2.0f)
	{}

	void HidePlaceOpener::OnLateStart() {
		m_allHidePlaces = maru::Utility::ConvertArraySharedToWeak(maru::Utility::FindComponents<HidePlace>(GetStage()));
	}

	void HidePlaceOpener::OnUpdate() {
		for (auto& weakPlace : m_allHidePlaces) {
			auto place = weakPlace.lock();
			if (!place) {
				continue;
			}

			if (place->IsOpen()) 
			{
				if (m_isDrawUI)
				{
					place->SetDrawUI(false);
				}
				continue;
			}

			auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), place->GetGameObject());
			auto selfPosition = transform->GetPosition();
			auto placePosition = place->GetGameObject()->GetComponent<Transform>()->GetPosition();
			float range = GetRange();
			float length = toVec.length();
			bool isDraw = toVec.length() < GetRange();
			
			if (isDraw != place->IsDrawUI())
			{
				if (m_isDrawUI)
				{
					place->SetDrawUI(isDraw);
				}
				m_canOpenHidePlace = place;
			}
		}
	}
}