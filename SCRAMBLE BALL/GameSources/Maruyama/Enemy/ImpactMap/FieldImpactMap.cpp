
/*!
@file FieldImpactMap.cpp
@brief FieldImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "FieldImpactMap.h"

#include "Maruyama/Load/StageMapCSV.h"
#include "ImpactMap.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"

#include "Maruyama/Utility/MaruAction.h"
#include "Maruyama/Utility/Utility.h"

#include "OccupancyManager.h"

namespace basecross {

	namespace maru {

		std::weak_ptr<FieldImpactMap> SingletonComponent<FieldImpactMap>::sm_instance;

		//--------------------------------------------------------------------------------------
		///	フィールド影響マップのパラメータ
		//--------------------------------------------------------------------------------------

		FieldImpactMap_Parametor::FieldImpactMap_Parametor():
			intervalRange(10.0f),
			createHeightOffset(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		///	フィールド影響マップ本体
		//--------------------------------------------------------------------------------------

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors
		):
			FieldImpactMap(objPtr, floors, Parametor())
		{}

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors, 
			const Parametor& parametor
		):
			SingletonComponent(objPtr), 
			m_floors(maru::Utility::ConvertArraySharedToWeak(floors)), 
			m_param(parametor),
			m_impactMap(new ImpactMap(GetGameObject()->GetStage())),
			m_occupancyManager(new OccupancyManager(m_impactMap))
		{}

		void FieldImpactMap::OnLateStart() {
			//m_impactMap = std::make_shared<ImpactMap>(GetStage()); //ImpactMapの生成

			m_impactMap->ClearNodes();  //ノードのクリア
			m_impactMap->ClearEdges();  //エッジのクリア
			const float IntervalRange = m_param.intervalRange;	//影響マップの各ノードの距離
			SettingImpactMap(m_floors, IntervalRange);			//影響マップのセッティング

			//m_impactMap->GetGraphAstar()->CreateAreaAstarGraph();	//それぞれのエリアのグラフの中心位置を設定する。
				//エリアグラフ用のエッジをつなぐ

			m_impactMap->CreateDebugDraw(false);		//デバッグデータの生成
		}

		void FieldImpactMap::OnUpdate() {
			OccupancyUpdate();

			//デバッグ
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsYDown(); });
			}
		}

		void FieldImpactMap::OccupancyUpdate() {
			if (m_occupancyManager) {
				m_occupancyManager->OnUpdate();
			}
		}

		std::vector<Vec3> FieldImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			return m_impactMap->GetRoutePositons(selfPosition, targetPosition);
		}

		std::vector<Vec3> FieldImpactMap::GetRoutePositions(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex) {
			return m_impactMap->GetRoutePositions(selfPosition, targetPosition, areaIndex);
		}

		std::shared_ptr<ImpactMap> FieldImpactMap::GetImpactMap() const {
			return m_impactMap;
		}

		int FieldImpactMap::SearchNearAreaIndex(const Vec3& position) const {
			return m_impactMap->GetGraphAstar()->SearchNearAreaIndex(position);
		}

		std::vector<int> FieldImpactMap::SearchAreaRouteIndices(const Vec3& startPosition, const Vec3& targetPosition) {
			return m_impactMap->GetGraphAstar()->SearchAreaRouteIndices(startPosition, targetPosition);
		}

		void FieldImpactMap::AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node) {
			if (m_occupancyManager) {
				m_occupancyManager->AddOccupancyUpdateData(node);
			}
		}

	}
}