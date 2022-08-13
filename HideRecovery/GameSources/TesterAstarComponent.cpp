
/*!
@file TesterAstarComponent.cpp
@brief TesterAstarComponentのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TesterAstarComponent.h"

#include "MaruUtility.h"

#include "PlayerInputer.h"

#include "ImpactMap.h"
#include "SingletonComponent.h"
#include "FieldImpactMap.h"

#include "EyeSearchRange.h"
#include "NavGraphNode.h"

namespace basecross {
	namespace Tester {

		void TesterAstarComponent::OnUpdate() {
			if (PlayerInputer::GetInstance()->IsLeftDown()) {
				//UpdateEyeRangeImpactMap();
			}

			UpdateEyeRangeImpactMap();
		}

		void TesterAstarComponent::UpdateEyeRangeImpactMap() {
			std::shared_ptr<maru::ImpactMap> impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();

			auto nodes = impactMap->GetEyeRangeNodes(transform->GetPosition(), GetThis<I_Impacter>());

			for (auto node : nodes) {
				//占有値更新
				auto impactData = node->GetImpactData();
				impactData.occupancyValue = 0;
				node->SetImpactData(impactData);

				//占有値に合わせてノードの色変更（デバッグ）
				auto colorValue = 1.0f - impactData.occupancyValue;
				impactMap->ChangeTargetNodeColor(node->GetIndex(), Col4(1.0f, colorValue, colorValue, 1.0f));
			}
		}

		std::shared_ptr<GameObject> TesterAstarComponent::GetImpacterObject() const noexcept {
			return GetGameObject();
		}

		std::shared_ptr<EyeSearchRange> TesterAstarComponent::GetEyeSearchRange() const {
			return GetGameObject()->GetComponent<EyeSearchRange>();
		}

		std::shared_ptr<maru::ImpactMap> TesterAstarComponent::GetImpactMap() const {
			return maru::FieldImpactMap::GetInstance()->GetImpactMap();
		}
	}
}

//endbasecross