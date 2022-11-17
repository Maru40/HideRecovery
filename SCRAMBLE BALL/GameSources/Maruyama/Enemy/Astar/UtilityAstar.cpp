/*!
@file UtilityAstar.cpp
@brief UtilityAstar実体
*/

#include "UtilityAstar.h"
#include "Maruyama/Utility/Utility.h"

#include "AstarEdge.h"
#include "SparseGraph.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(const GraphAstar& astar, const std::shared_ptr<GameObject>& target) {
		vector<std::shared_ptr<GameObject>> excluteObjs;

		auto targetPos = target->GetComponent<Transform>()->GetPosition();
		return SearchNearNode(astar, targetPos);
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(const GraphAstar& astar, const Vec3& targetPos)
	{
		return SearchNearNode(astar.GetGraph(), targetPos);
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(
		const std::shared_ptr<const GraphAstar::GraphType>& graph, 
		const Vec3& targetPos, 
		const bool isObstacleConfirmation
	) {
		//将来的にここではobjectsを探さない。
		//auto objects = maru::UtilityObstacle::FindObstacles(maru::Utility::GetStage()->GetGameObjectVec());
		return SearchNearNode(graph, targetPos, {}, {}, isObstacleConfirmation);
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(
		const std::shared_ptr<const GraphAstar::GraphType>& graph,
		const std::shared_ptr<GameObject>& targetObject,
		const std::vector<std::shared_ptr<GameObject>>& obstacleObjects,
		const std::vector<std::shared_ptr<GameObject>>& excludeObstacleObject,
		const bool isObstacleConfirmation
	) {
		auto targetPosition = targetObject->GetComponent<Transform>()->GetPosition();
		return SearchNearNode(graph, targetPosition, obstacleObjects, excludeObstacleObject, isObstacleConfirmation);
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(
		const std::shared_ptr<const GraphAstar::GraphType>& graph,
		const Vec3& targetPosition,
		const std::vector<std::shared_ptr<GameObject>>& obstacleObjects,
		const std::vector<std::shared_ptr<GameObject>>& excludeObstacleObject,
		const bool isObstacleConfirmation
	) {
		const auto& nodes = graph->GetNodes();	//ノード配列の取得

		float minRange = FLT_MAX;
		std::shared_ptr<NavGraphNode> minNode = nullptr;  //一番距離が短いノード
		//障害物から障害物から排除したいオブジェクトを省く
		auto objects = maru::Utility::ExcludeVector(obstacleObjects, excludeObstacleObject);

		//検索開始
		for (const auto& node : nodes) {
			auto pos = node->GetPosition();
			auto toNode = pos - targetPosition;
			const auto& range = toNode.length();

			int index = node->GetIndex();

			//障害物が合ったらコンティニュ―
			if (isObstacleConfirmation && maru::UtilityObstacle::IsRayObstacle(targetPosition, pos, objects)) {
				continue;
			}

			//距離が短かったらこれにする。
			if (range <= minRange) {
				minRange = range;
				minNode = node;
			}
		}

		if (minNode == nullptr) {
			Debug::GetInstance()->Log(L"ノードが検索できませんでした。");
		}

		return minNode;
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchMyNodeToTargetNearNode(const GraphAstar& astar,
		const std::shared_ptr<GameObject>& selfObject, const std::shared_ptr<GameObject>& target)
	{
		//自分自身の近くのノードの検索
		const std::shared_ptr<const NavGraphNode> selfNode = SearchNearNode(astar, selfObject);
		const int from = selfNode->GetIndex();  //そのインデクスの取得

		const auto& graph = astar.GetGraph();
		const auto& edges = graph->GetEdges(from);
		const Vec3 targetPosition = target->GetComponent<Transform>()->GetPosition();

		float minRange = FLT_MAX;
		std::shared_ptr<NavGraphNode> nearNode;
		for (auto& edge : edges) {
			int toIndex = edge->GetTo();
			std::shared_ptr<NavGraphNode> toNode = graph->GetNode(toIndex);
			auto toVec = toNode->GetPosition() - targetPosition;
			if (toVec.length() < minRange) {
				nearNode = toNode;
				minRange = toVec.length();
			}
		}

		return nearNode;
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::CalculateTargetDirectNode(
		const GraphAstar& astar,
		const std::shared_ptr<NavGraphNode>& startNode,
		const Vec3& targetPos)
	{
		const auto& graph = astar.GetGraph();
		const auto& edges = graph->GetEdges(startNode->GetIndex());

		auto startNodePos = startNode->GetPosition();
		auto toTargetVec = targetPos - startNodePos;
		
		float minRad = 360.0f;
		std::shared_ptr<NavGraphNode> reNode;
		for (const auto& edge : edges) {
			auto nextNode = graph->GetNode(edge->GetTo());
			auto nextPos = nextNode->GetPosition();

			auto toNextNodeVec = nextPos - startNodePos;
			toNextNodeVec.y = 0.0f;

			//角度を求める。
			auto newDot = dot(toTargetVec.GetNormalized(), toNextNodeVec.GetNormalized());
			auto newRad = acosf(newDot);

			if (newRad < minRad) {	//角度が小さいなら
				minRad = newRad;
				reNode = nextNode;
			}
		}

		return reNode;
	}

	//逃げるためのノードの検索
	std::shared_ptr<NavGraphNode> UtilityAstar::CalculateTargetEscapeDirectNode(const GraphAstar& astar,
		const std::shared_ptr<NavGraphNode>& startNode,
		const Vec3& targetPos)
	{
		const auto& graph = astar.GetGraph();
		const auto& edges = graph->GetEdges(startNode->GetIndex());

		auto startNodePos = startNode->GetPosition();
		auto toTargetVec = targetPos - startNodePos;

		float maxRad = 0.0f;
		std::shared_ptr<NavGraphNode> reNode;
		for (const auto& edge : edges) {
			int toIndex = edge->GetTo();
			std::shared_ptr<NavGraphNode> nextNode = graph->GetNode(toIndex);
			auto nextPos = nextNode->GetPosition();

			auto toNextNodeVec = nextPos - startNodePos;
			toNextNodeVec.y = 0.0f;

			auto newDot = dot(toTargetVec.GetNormalized(), toNextNodeVec.GetNormalized());
			auto newRad = acosf(newDot);

			if (newRad > maxRad) {
				maxRad = newRad;
				reNode = nextNode;
			}
		}

		return reNode;
	}
}

//endbasecross