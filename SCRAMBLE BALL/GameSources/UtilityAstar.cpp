/*!
@file UtilityAstar.cpp
@brief UtilityAstar����
*/

#include "UtilityAstar.h"
#include "MaruUtility.h"

#include "AstarEdge.h"
#include "SparseGraph.h"
#include "UtilityObstacle.h"

namespace basecross {

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(const GraphAstar& astar, const std::shared_ptr<GameObject>& target) {
		vector<std::shared_ptr<GameObject>> excluteObjs;

		auto targetPos = target->GetComponent<Transform>()->GetPosition();
		return SearchNearNode(astar, targetPos);
	}

	std::shared_ptr<NavGraphNode> UtilityAstar::SearchNearNode(const GraphAstar& astar, const Vec3& targetPos)
	{
		const auto& graph = astar.GetGraph();
		const auto& nodes = graph->GetNodes();

		float minRange = 10000.0f;
		std::shared_ptr<NavGraphNode> minNode = nullptr;  //��ԋ������Z���m�[�h
		
		//�����J�n
		auto objects = maru::Utility::GetStage()->GetGameObjectVec();
		for (const auto& node : nodes) {
			auto pos = node->GetPosition();
			auto toNode = pos - targetPos;
			const auto& range = toNode.length();

			//��Q������������R���e�B�j���\
			if (maru::UtilityObstacle::IsRayObstacle(targetPos, pos, objects)) {
				continue;
			}

			//�������Z�������炱��ɂ���B
			if (range <= minRange) {
				minRange = range;
				minNode = node;
			}
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

		float minRange = 100000.0f;
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