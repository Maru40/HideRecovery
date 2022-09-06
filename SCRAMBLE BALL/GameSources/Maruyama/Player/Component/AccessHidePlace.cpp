
/*!
@file AccessHidePlace.cpp
@brief AccessHidePlaceクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AccessHidePlace.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/StageObject/HidePlace.h"

namespace basecross {

	AccessHidePlace::AccessHidePlace(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_range(2.0f)
	{}

	void AccessHidePlace::OnLateStart() {
		m_allHidePlaces = maru::Utility::ConvertArraySharedToWeak(maru::Utility::FindComponents<HidePlace>(GetStage()));
	}

	void AccessHidePlace::OnUpdate() {
		for (auto& weakPlace : m_allHidePlaces) {
			auto place = weakPlace.lock();
			if (!place) {
				continue;
			}

			if (place->IsOpen()) {
				place->SetDrawUI(false);
				continue;
			}

			auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), place->GetGameObject());
			auto selfPosition = transform->GetPosition();
			auto placePosition = place->GetGameObject()->GetComponent<Transform>()->GetPosition();
			float range = GetRange();
			float length = toVec.length();
			bool isDraw = toVec.length() < GetRange();
			
			if (isDraw != place->IsDrawUI()) {
				place->SetDrawUI(isDraw);
			}
		}
	}

	void AccessHidePlace::Access() {
		for (auto& weakPlace : m_allHidePlaces) {
			auto place = weakPlace.lock();
			if (!place) {
				continue;
			}

			auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), place->GetGameObject());
			if (toVec.length() < GetRange()) {
				place->Open();
			}
		}
	}

}