
/*!
@file SoleImpactMap.cpp
@brief SoleImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SoleImpactMap.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"

#include "Maruyama/Interface/I_Impact.h"
#include "Maruyama/Enemy/ImpactMap/ImpactManager.h"

namespace basecross {

	namespace maru {

		SoleImpactMap::SoleImpactMap(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr)
		{}

		SoleImpactMap::SoleImpactMap(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
			Component(objPtr), m_param(parametor)
		{}

		void SoleImpactMap::OnCreate() {
			CreateImpactMap();
		}

		void SoleImpactMap::OnUpdate() {
			//デバッグ用
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsXDown(); });
			}
		}

		void SoleImpactMap::CreateImpactMap() {
			auto impactMap = std::make_shared<ImpactMap>(GetStage(), m_param.factoryParam);
			impactMap->GetGraphAstar()->SetNodesParent(GetGameObject());
			impactMap->CreateDebugDraw(false);  //デバッグ表示を生成

			m_impactMap = impactMap;
		}

		void SoleImpactMap::CalculateImpact(const std::shared_ptr<I_Impacter>& impacter) {
			//影響マップ管理クラスの生成
			auto impactManager = std::make_shared<ImpactManager>();

			//影響マップ管理クラスに計算させる。
			impactManager->CalculateAllImpact(m_impactMap, impacter);
		}


		//①ノードをプレイヤーの相対位置に合わせて移動させる。
		//②
		
	}
}