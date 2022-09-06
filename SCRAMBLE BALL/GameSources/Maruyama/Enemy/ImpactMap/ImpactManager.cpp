
/*!
@file ImpactManager.cpp
@brief ImpactManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ImpactManager.h"

#include "SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "ImpactMap.h"

#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "SparseGraph.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	namespace maru {

		//ノードのアクティブ状態更新
		void ImpactManager::NodeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			
		}
		
		//エッジのアクティブ状態更新
		void ImpactManager::EdgeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//障害物がある場合はActiveをfalseに変更
			const auto& astar = impactMap->GetGraphAstar();
			const auto& graph = astar->GetGraph();
			const auto numNode = graph->GetNumNodes();

			for (int i = 0; i < numNode; i++) {
				for (const auto& edge : graph->GetEdges(i)) {
					auto fromNode = graph->GetNode(edge->GetFrom());
					auto toNode = graph->GetNode(edge->GetTo());
					
					//障害物に阻まれていたら非アクティブ状態にする。
					if (maru::UtilityObstacle::IsRayObstacle(fromNode->GetPosition(), toNode->GetPosition(), impactMap->GetStage()->GetGameObjectVec())) {
						edge->SetIsActive(false);
					}
				}
			}
		}

		//自分の周囲に影響度を更新する。
		void ImpactManager::CalculateAroundImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			auto data = impacter->GetImpacterData();

			//自分の所属するノードの取得
			auto selfNode = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), impacter->GetImpacterObject());

			//自分の周囲から近い部分ほど影響度をアップさせる。
			
		}

		//視界範囲の影響度を更新する
		void ImpactManager::CalculateEyeImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//視界範囲の取得
			auto impacterObject = impacter->GetImpacterObject();
			auto impacterTrans = impacterObject->GetComponent<Transform>();
			auto eye = impacter->GetEyeSearchRange();
			EyeSearchRangeParametor eyeParam = eye->GetParametor();

			//自分の所属するノードの取得
			auto selfNode = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), impacter->GetImpacterObject());

			//視界内のノードを取得
			auto nodes = impactMap->GetEyeRangeNodes(impacterTrans->GetPosition(), impacter);

			//視界内の影響度を更新
			for (auto& node : nodes) {
				//占有値更新
				auto impactData = node->GetImpactData();
				impactData.occupancyValue = 0.0f;
				node->SetImpactData(impactData);

				if (auto fieldImpact = maru::FieldImpactMap::GetInstance()) {
					fieldImpact->AddOccupancyUpdateData(node);
				}
			}
		}

		//影響度の計算
		void ImpactManager::CalculateAllImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//ノードのアクティブ状態の更新
			NodeActiveUpdate(impactMap, impacter);

			//エッジのアクティブ状態の更新
			EdgeActiveUpdate(impactMap, impacter);

			auto impactType = impacter->GetImpacterData().type;
			switch (impactType)
			{
			case ImpacterType::Circle:
				CalculateAroundImpact(impactMap, impacter);
				break;

			case ImpacterType::Eye:
				CalculateAroundImpact(impactMap, impacter);
				CalculateEyeImpact(impactMap, impacter);
				break;
			}
		}

	}
}