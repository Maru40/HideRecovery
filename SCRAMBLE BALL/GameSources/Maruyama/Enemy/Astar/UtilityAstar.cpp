/*!
@file UtilityAstar.cpp
@brief UtilityAstar����
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
		//�����I�ɂ����ł�objects��T���Ȃ��B
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
		const auto& nodes = graph->GetNodes();	//�m�[�h�z��̎擾

		float minRange = FLT_MAX;
		std::shared_ptr<NavGraphNode> minNode = nullptr;  //��ԋ������Z���m�[�h
		//��Q�������Q������r���������I�u�W�F�N�g���Ȃ�
		auto objects = maru::Utility::ExcludeVector(obstacleObjects, excludeObstacleObject);

		//�����J�n
		for (const auto& node : nodes) {
			auto pos = node->GetPosition();
			auto toNode = pos - targetPosition;
			const auto& range = toNode.length();

			int index = node->GetIndex();

			//��Q������������R���e�B�j���\
			if (isObstacleConfirmation && maru::UtilityObstacle::IsRayObstacle(targetPosition, pos, objects)) {
				continue;
			}

			//�������Z�������炱��ɂ���B
			if (range <= minRange) {
				minRange = range;
				minNode = node;
			}
		}

		if (minNode == nullptr) {
			Debug::GetInstance()->Log(L"�m�[�h�������ł��܂���ł����B");
		}

		return minNode;
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchMyNodeToTargetNearNode(const GraphAstar& astar,
		const std::shared_ptr<GameObject>& selfObject, const std::shared_ptr<GameObject>& target)
	{
		//�������g�̋߂��̃m�[�h�̌���
		const std::shared_ptr<const NavGraphNode> selfNode = SearchNearNode(astar, selfObject);
		const int from = selfNode->GetIndex();  //���̃C���f�N�X�̎擾

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

			//�p�x�����߂�B
			auto newDot = dot(toTargetVec.GetNormalized(), toNextNodeVec.GetNormalized());
			auto newRad = acosf(newDot);

			if (newRad < minRad) {	//�p�x���������Ȃ�
				minRad = newRad;
				reNode = nextNode;
			}
		}

		return reNode;
	}

	//�����邽�߂̃m�[�h�̌���
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