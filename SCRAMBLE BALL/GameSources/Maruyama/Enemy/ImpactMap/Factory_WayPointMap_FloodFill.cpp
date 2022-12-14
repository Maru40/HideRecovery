
/*!
@file ImpactMap.cpp
@brief ImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/StateMachine/NodeBase.h"
#include "Maruyama/StateMachine/EdgeBase.h"
#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Factory_WayPointMap_FloodFill.h"

#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	namespace maru {

		//方向配列
		const Vec3 DIRECTIONS[] = {
			Vec3::Right(),										//右
			(Vec3::Right() + Vec3::Forward()).GetNormalized(),	//右奥
			(Vec3::Right() - Vec3::Forward()).GetNormalized(),	//右手前

			-Vec3::Right(),										//左
			(-Vec3::Right() + Vec3::Forward()).GetNormalized(),	//左奥
			(-Vec3::Right() - Vec3::Forward()).GetNormalized(),	//左手前

			Vec3::Forward(),									//奥
			-Vec3::Forward()									//手前
		};


		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム本体
		//--------------------------------------------------------------------------------------

		Factory_WayPointMap_FloodFill::Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage) :
			m_stage(stage)
		{
			/*
			auto startPosition = Vec3(0.0f);

			for (const auto& direction : DIRECTIONS) {
				auto targetPosition = startPosition + (direction * 2.0f);

				auto object = stage->Instantiate<GameObject>(targetPosition, Quat::Identity());
				object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
			}
			*/
		}

		bool Factory_WayPointMap_FloodFill::IsCreate(
			const Vec3& startPosition,
			const Vec3& targetPosition,
			const Parametor& parametor
		) {
			//障害物に当たっていたら
			auto obstacleObjects = GetStage()->GetGameObjectVec();	//障害物配列
			if (maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, obstacleObjects)) {
				return false;	//生成できない
			}

			return true;	//全ての条件を得ているため、生成できる。
		}

		std::vector<Vec3> Factory_WayPointMap_FloodFill::CalculationTargetPositions(
			const Vec3& startPosition, 
			const Parametor& parametor
		) {
			std::vector<Vec3> result;

			for (const auto& direction : DIRECTIONS) {
				//生成したい場所を設定
				auto targetPosition = startPosition + (direction * parametor.intervalRange);

				//その場所に生成できるか判断
				if (IsCreate(startPosition, targetPosition, parametor)) {
					result.push_back(targetPosition);
				}
			}

			return result;
		}

		void Factory_WayPointMap_FloodFill::CreateWayPoints(
			const Vec3& startPosition,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			auto targetPositions = CalculationTargetPositions(startPosition, parametor);

			for (const auto& targetPosition : targetPositions) {
				//graph->AddNode(targetPosition);	//ウェイポイントの生成
				//graph->AddNode();
				//graph->AddNode(0, std::make_shared<NodeBase>());
				graph->AddNode<AstarNode>(targetPosition);
				//graph->AddEdge<AstarEdge>();
				
				//graph->AddEdge();
			}
		}

		void Factory_WayPointMap_FloodFill::AddWayPointMap(
			const Vec3& baseStartPosition,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			std::queue<Vec3> startPositionQueue;
			startPositionQueue.push(baseStartPosition);

			while (!startPositionQueue.empty()) {	//キューが空になるまで
				auto startPosition = startPositionQueue.front();
				startPositionQueue.pop();
				CreateWayPoints(startPosition, graph ,parametor);
			}
		}

	}
}