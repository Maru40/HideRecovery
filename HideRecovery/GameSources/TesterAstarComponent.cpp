
/*!
@file TesterAstarComponent.cpp
@brief TesterAstarComponent‚ÌƒNƒ‰ƒXŽÀ‘Ì
’S“–FŠÛŽR—TŠì
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
				UpdateEyeRangeImpactMap();
			}
		}

		void TesterAstarComponent::UpdateEyeRangeImpactMap() {
			std::shared_ptr<maru::ImpactMap> impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();

			auto nodes = impactMap->GetEyeRangeNodes(transform->GetPosition(), GetThis<I_Impacter>());

			for (auto node : nodes) {
				impactMap->ChangeTargetNodeColor(node->GetIndex(), Col4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		std::shared_ptr<GameObject> TesterAstarComponent::GetImpacterObject() const noexcept {
			return GetGameObject();
		}

		std::shared_ptr<EyeSearchRange> TesterAstarComponent::GetEyeSearchRange() const {
			return GetGameObject()->GetComponent<EyeSearchRange>();
		}
	}
}

//endbasecross