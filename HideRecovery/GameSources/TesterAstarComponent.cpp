
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

#include "ImpactManager.h"
#include "I_Impact.h"

namespace basecross {
	namespace Tester {

		void TesterAstarComponent::OnUpdate() {
			if (PlayerInputer::GetInstance()->IsLeftDown()) {
				//UpdateEyeRangeImpactMap();
			}

			UpdateEyeRangeImpactMap();
		}

		void TesterAstarComponent::UpdateEyeRangeImpactMap() {
			auto impacter = GetGameObject()->GetComponent<I_Impacter>(false);
			if (!impacter) {
				return;
			}

			maru::ImpactManager impactManager;
			std::shared_ptr<maru::ImpactMap> impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();

			impactManager.CalculateEyeImpact(impactMap, impacter);
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