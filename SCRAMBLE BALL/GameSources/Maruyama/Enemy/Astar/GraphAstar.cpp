/*!
@file GraphAstar.cpp
@brief GraphAstar�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Heuristic.h"
#include "SparseGraph.h"
#include "NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"
#include "GraphAstar.h"
#include "Maruyama/Utility/Mathf.h"
#include "Maruyama/Utility/Random.h"

#include "Maruyama/Utility/Component/CollisionAction.h"

#include "Maruyama/Debug/Object/DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^
	//--------------------------------------------------------------------------------------

	OpenData::OpenData()
		:OpenData(nullptr, 0, 0)
	{}
	OpenData::OpenData(const std::shared_ptr<NavGraphNode>& node, const float& range, const float& heuristic)
		:node(node), range(range), heuristic(heuristic), isActive(true)
	{}

	float OpenData::GetSumRange() const {
		return range + heuristic;
	}

	//--------------------------------------------------------------------------------------
	///	GraphAstar�̖{��
	//--------------------------------------------------------------------------------------

	GraphAstar::GraphAstar(const std::shared_ptr<GraphType>& graph)
		:m_baseGraph(graph), m_heuristic(new Heuristic())
	{}

	Vec3 GraphAstar::CalucTargetNode(const std::shared_ptr<GameObject>& objPtr) {
		if (m_route.size() == 0) {
			m_isRouteEnd = true;
		}

		if (m_isRouteEnd) {
			return Vec3(0.0f);
		}

		//�I�u�W�F�N�g���T���m�[�h�̋߂��ɂ�����A���̃m�[�h�ɐ؂�ւ���B
		float nearRange = 2.0f;  //�߂��Ɣ��f����鋗��

		auto transform = objPtr->GetComponent<Transform>();
		auto objPosition = transform->GetPosition();

		auto nodePosition = m_route.top()->GetPosition();  //���݂̖ڕW�m�[�h�̃|�W�V�������擾

		auto toNode = nodePosition - objPosition;
		if (toNode.length() <= nearRange) {  //�m�[�h�̋߂��܂ŗ�����B
			m_route.pop();
		}

		//�m�[�h�C���f�b�N�X���V���[�g�𒴂�����
		if (m_route.size() == 0) {
			m_isRouteEnd = true;//�@�\��off�ɂ���B
		}

		return nodePosition;
	}

	void GraphAstar::SearchAstarStart(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target) {
		auto selfNearNode = UtilityAstar::SearchNearNode(*this, self);
		auto targetNearNode = UtilityAstar::SearchNearNode(*this, target);
		SearchAstarStart(selfNearNode, targetNearNode);
	}

	void GraphAstar::SearchAstarStart(const std::shared_ptr<GameObject>& self, const Vec3& targetPos) {
		auto selfNearNode = UtilityAstar::SearchNearNode(*this, self);
		auto targetNearNode = UtilityAstar::SearchNearNode(*this, targetPos);
		SearchAstarStart(selfNearNode, targetNearNode);
	}

	void GraphAstar::SearchAstarStart(const Vec3& selfPos, const Vec3& targetPos) {
		auto selfNearNode = UtilityAstar::SearchNearNode(*this, selfPos);
		auto targetNearNode = UtilityAstar::SearchNearNode(*this, targetPos);

		SearchAstarStart(selfNearNode, targetNearNode);
	}

	std::vector<Vec3> GraphAstar::CalculateRandomRoute(const Vec3& selfPosition) {
		if (m_baseGraph->GetNodes().size() == 0) {
			DebugObject::AddString(L"GraphAstar::CalculateRandomRoute(), GraphNode�����݂��܂���B");
			return vector<Vec3>();
		}

		int maxSize = (int)m_baseGraph->GetNodes().size() - 1;
		auto ramdomValue = maru::MyRandom::Random(0, maxSize);

		auto selfNode = UtilityAstar::SearchNearNode(*this, selfPosition);
		auto targetNode = m_baseGraph->GetArrayNumberNode(ramdomValue);
		SearchAstarStart(selfNode, targetNode);

		return GetRoutePositions();
	}

	void GraphAstar::SearchAstarStart(const std::shared_ptr<NavGraphNode>& selfNearNode, const std::shared_ptr<NavGraphNode>& targetNearNode) {
		if (selfNearNode == nullptr || targetNearNode == nullptr) {
			DebugObject::AddString(L"GraphAstar::SearchAstarStart(), node��null�ł�");
			return;
		}

		ResetAstar();

		m_heuristic->SetTargetNode(targetNearNode);  //�q���[���X�e�B�b�N�֐��ɖڕW�m�[�h��ݒ�

		if (selfNearNode->GetPosition() == targetNearNode->GetPosition()) {
			m_route.push(selfNearNode);
			return;
		}

		//���[�v���ď������s���B
		LoopSearchAstar(selfNearNode, targetNearNode);
	}

	void GraphAstar::LoopSearchAstar(const std::shared_ptr<NavGraphNode>& initialNode, const std::shared_ptr<NavGraphNode>& targetNode) {
		int tempIndex = 0;
		int maxTempIndex = 1000;
		//�O���t�̒��g�R�s�[
		auto graph = CreateCopyGraph();
		m_openDataMap.clear();
		m_debugIndices.clear();
		m_openDataMap[initialNode->GetIndex()] = OpenData(initialNode, 0, m_heuristic->CalculateHeuristicRange(targetNode));
		//�����I���̃o�O�Ȃ��Ȃ���������B
		while (tempIndex < maxTempIndex) {
			//�I�[�v���f�[�^�����p�̊�m�[�h�̐����B
			auto baseNode = CalculateCreateOpenDataBaseNode(initialNode);
			m_debugIndices.push_back(baseNode->GetIndex());
			//�I�[�v���f�[�^�̐����B�^�[�Q�b�g�m�[�h�ɂ��ǂ蒅������true��Ԃ��B
			if (CreateOpenData(baseNode, graph)) {
				break;
			}

			tempIndex++;
		}

		//�I�[�v���f�[�^����ŒZ�o�H���擾
		m_route.push(targetNode);
		m_openDataMap[targetNode->GetIndex()].isActive = false;
		CreateRoute(initialNode, targetNode, graph);

		//�f�o�b�O---------------------------------------------

		//���[�v�̏���𒴂������Ƃ�`����B
		//if (tempIndex >= maxTempIndex) {
		//	DebugObject::sm_wss << endl << L"serchOver" << endl;
		//	m_debugIndices;
		//}

		//DebugDraw(); //��������Route��\������B
		//-----------------------------------------------------
	}

	std::shared_ptr<NavGraphNode> GraphAstar::CalculateCreateOpenDataBaseNode(const std::shared_ptr<NavGraphNode>& initialNode) {
		if (m_openDataMap.size() == 0) {
			return initialNode;
		}

		OpenData resultData(nullptr, FLT_MAX, FLT_MAX);
		for (const auto& pair : m_openDataMap) {
			if (!pair.second.node->IsActive()) {
				continue;
			}

			//�l���������ꍇ
			if (pair.second.GetSumRange() < resultData.GetSumRange()) {
				resultData = pair.second;  //Result���ɓ����B
			}
		}

		return resultData.node.GetShard() == nullptr ? initialNode : resultData.node.GetShard();
	}

	bool GraphAstar::CreateOpenData(const ex_weak_ptr<NavGraphNode>& baseNode, const std::shared_ptr<GraphType>& graph) {
		//bool isArriveTargetNode = false;  //�^�[�Q�b�g�m�[�h�ɂ��ǂ蒅�������ǂ���
		auto edges = graph->GetEdges(baseNode->GetIndex());	//�G�b�W�̎擾

		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());
			if (!node->IsActive()) {
				continue;
			}

			auto toNodeVec = baseNode->GetPosition() - node->GetPosition();	//�m�[�h�ւ̃x�N�g�����擾
			auto range = toNodeVec.length();
			auto heuristic = m_heuristic->CalculateHeuristicRange(node);	//�q���[�X���b�N�����̎擾

			auto newData = OpenData(node, range, heuristic);	//�V�K�I�[�v���f�[�^�̐���
			m_openDataMap[node->GetIndex()] = newData;

			//heuristic������Ȃ�������������^�[�Q�b�g�ɂ��ǂ蒅�������߁A�I���B
			constexpr float NearRange = 0.1f;
			if (newData.heuristic < NearRange) {	
				return true;
			}
		}

		baseNode->SetIsActive(false);
		return false;
	}

	void GraphAstar::CreateRoute(
		const std::shared_ptr<NavGraphNode>& initialNode,
		const std::shared_ptr<NavGraphNode>& targetNode,
		const std::shared_ptr<GraphType>& graph)
	{
		auto edges = graph->GetEdges(targetNode->GetIndex());

		auto resultData = new OpenData(nullptr, FLT_MAX, FLT_MAX);
		auto savePtr = resultData;
		//�m�[�h�̒��ň�ԋ߂������擾
		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());
			//�m�[�h�����݂��Ȃ��A�܂��́A�f�[�^����A�N�e�B�u�Ȃ�A�������΂��B
			if (m_openDataMap.count(node->GetIndex()) == 0 || !m_openDataMap[node->GetIndex()].isActive) {
				continue;
			}

			auto& data = m_openDataMap[node->GetIndex()];
			if (data.GetSumRange() < resultData->GetSumRange()) {
				resultData = &data;
			}
		}

		if (resultData->node.GetShard() == nullptr) {
			return;
		}

		//�����m�[�h�Ȃ�
		if (resultData->node == initialNode) {
			return; //��������߂�B
		}

		m_route.push(resultData->node.GetShard());
		resultData->isActive = false;
		delete savePtr;

		CreateRoute(initialNode, resultData->node.GetShard(), graph);
	}

	//�����I�Ɋg��(Node�̃|�C���^�Q�Ƃ�؂�ւ������B)
	std::shared_ptr<GraphAstar::GraphType> GraphAstar::CreateCopyGraph() {
		auto graph = std::make_shared<GraphType>(true);

		int index = 0;
		for (auto& baseNode : m_baseGraph->GetNodes()) {
			graph->SetNodes(m_baseGraph->GetNodesCopy());
		}

		graph->SetEdgesMap(m_baseGraph->GetEdgesMapCopy());
		return graph;
	}

	//�A�N�Z�b�T------------------------------------------------------------------------------------------------------

	std::vector<std::shared_ptr<NavGraphNode>> GraphAstar::GetRouteVec() const {
		std::vector<std::shared_ptr<NavGraphNode>> resultVec;
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			resultVec.push_back(copyRoute.top());
			copyRoute.pop();
		}

		return resultVec;
	}

	std::vector<Vec3> GraphAstar::GetRoutePositions() const {
		std::vector<Vec3> resultPositions;
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			resultPositions.push_back(copyRoute.top()->GetPosition());
			copyRoute.pop();
		}

		return resultPositions;
	}

	//int GraphAstar::AddNode(const Vec3& position) {
	//	return AddNode(position);
	//	////�m�[�h�̐���
	//	//auto index = m_baseGraph->GetNextFreeNodeIndex();
	//	//auto newNode = std::make_shared<NavGraphNode>(index, position);

	//	//m_baseGraph->AddNode(newNode);

	//	//return index++;
	//}

	int GraphAstar::AddNode(const Vec3& position,
		const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
	{
		//�m�[�h�̐���
		auto index = m_baseGraph->GetNextFreeNodeIndex();
		auto newNode = make_shared<NavGraphNode>(index, position);

		auto edges = UtilityAstar::CreateAdjacendEdges(m_baseGraph, newNode, obstacleObjs, excluteObjs);

		//���݂̓G�b�W���쐬���ꂽ�Ƃ��̂ݐ�������悤�ɂ���B(�q�����ĂȂ��m�[�h��Ώۂɂ����ꍇ�ɏ������~�܂��Ă��܂����߁B)
		//���P���ꎟ��O���B
		if (edges.size() != 0) {
			//�m�[�h�̒ǉ�
			m_baseGraph->AddNode(newNode);
		}

		return index++;
	}

	void GraphAstar::RemoveNode(const int& index) {
		m_baseGraph->RemoveNode(index);
	}

	void GraphAstar::SetNodesParent(const std::shared_ptr<GameObject>& parent) {
		m_baseGraph->SetNodesParent(parent);
	}

	void GraphAstar::AddEdges() {
		maru::Action<void()> actions;

		auto nodes = m_baseGraph->GetNodes();
		for (auto& node : nodes) {
			auto edges = UtilityAstar::CreateAdjacendEdges<NavGraphNode, AstarEdge>(m_baseGraph, node);
			//�G�b�W������Ȃ�������m�[�h���폜
			if (edges.size() == 0) {
				//�m�[�h�̍폜���܂Ƃ߂�B
				actions.AddFunction([&, this]() {m_baseGraph->RemoveNode(node->GetIndex()); });
			}
		}

		actions.Invoke();
	}

	void GraphAstar::ClearNodes() {
		m_baseGraph->ClearNodes();
	}

	void GraphAstar::ClearEdges() {
		m_baseGraph->ClearEdges();
	}

	void GraphAstar::ResetAstar() {
		m_isRouteEnd = false;
		m_openDataMap.clear();
		maru::Utility::StackClear(m_route);
	}

	void GraphAstar::ResetAll() {
		ResetAstar();
		m_baseGraph->ResetAll();
		m_isRouteEnd = false;
	}


	//�f�o�b�O����---------------------------------------------------------------------------------------------

	void GraphAstar::DebugDraw() {
		//��������Route��\������B
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			DebugObject::AddValue(copyRoute.top()->GetIndex(), false);
			DebugObject::AddString(L",", false);
			copyRoute.pop();
		}
	}

}

//endbasecross