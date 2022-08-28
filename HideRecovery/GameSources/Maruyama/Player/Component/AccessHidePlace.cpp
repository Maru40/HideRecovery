
/*!
@file AccessHidePlace.cpp
@brief AccessHidePlaceクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AccessHidePlace.h"

#include "MaruUtility.h"
#include "HidePlace.h"

namespace basecross {

	AccessHidePlace::AccessHidePlace(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_range(2.0f)
	{}

	void AccessHidePlace::OnLateStart() {
		m_allHidePlaces = maru::Utility::ConvertArraySharedToWeak(maru::Utility::FindComponents<HidePlace>(GetStage()));
	}

	void AccessHidePlace::OnUpdate() {

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