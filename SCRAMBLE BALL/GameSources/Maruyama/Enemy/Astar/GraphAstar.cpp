/*!
@file GraphAstar.cpp
@brief GraphAstar�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Heuristic.h"
#include "SparseGraph.h"
#include "AstarGraph.h"
#include "NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"
#include "GraphAstar.h"
#include "Maruyama/Utility/Mathf.h"
#include "Maruyama/Utility/Random.h"

#include "OpenDataHandler.h"

#include "Maruyama/Utility/Component/CollisionAction.h"

#include "Maruyama/DebugClass/Object/DebugObject.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//class OpenDataHandle;

	//--------------------------------------------------------------------------------------
	///	GraphAstar�̖{��
	//--------------------------------------------------------------------------------------

	GraphAstar::GraphAstar(const std::shared_ptr<GraphType>& graph) :
		m_baseGraph(graph),
		m_areaIndexGraph(new AstarGraph(true))
	{}

	void GraphAstar::SearchAstarStart(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target) {
		SearchAstarStart(self->GetComponent<Transform>()->GetPosition(), target->GetComponent<Transform>()->GetPosition());
	}

	void GraphAstar::SearchAstarStart(const std::shared_ptr<GameObject>& self, const Vec3& targetPos) {
		SearchAstarStart(self->GetComponent<Transform>()->GetPosition(), targetPos);
	}

	void GraphAstar::SearchAstarStart(const Vec3& selfPos, const Vec3& targetPos) {
		auto selfNearNode = UtilityAstar::SearchNearNode(*this, selfPos);
		auto targetNearNode = UtilityAstar::SearchNearNode(*this, targetPos);

		SearchAstarStart(selfNearNode, targetNearNode, m_baseGraph);
	}

	void GraphAstar::SearchAstarStart(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex) {
		auto graph = GetGraph(areaIndex);

		auto selfNearNode = UtilityAstar::SearchNearNode(graph, selfPosition);
		int targetAreaIndex = SearchNearAreaIndex(targetPosition);
		bool isObstacleConfirmation = (areaIndex == targetAreaIndex);	//�G���A�������Ȃ�A��Q��������s���B
		auto targetNearNode = UtilityAstar::SearchNearNode(graph, targetPosition, isObstacleConfirmation);

		SearchAstarStart(selfNearNode, targetNearNode, m_baseGraph);
	}

	std::vector<Vec3> GraphAstar::CalculateRandomRoute(const Vec3& selfPosition) {
		if (m_baseGraph->GetNodes().size() == 0) {
			Debug::GetInstance()->Log(L"GraphAstar::CalculateRandomRoute(), GraphNode�����݂��܂���B");
			return vector<Vec3>();
		}

		int maxSize = (int)m_baseGraph->GetNodes().size() - 1;
		auto ramdomValue = maru::MyRandom::Random(0, maxSize);

		auto selfNode = UtilityAstar::SearchNearNode(*this, selfPosition);
		auto targetNode = m_baseGraph->GetArrayNumberNode(ramdomValue);
		SearchAstarStart(selfNode, targetNode, m_baseGraph);

		return GetRoutePositions();
	}

	std::vector<int> GraphAstar::SearchAreaIndexRoute(const Vec3& selfPosition, const Vec3& targetPosition) {
		auto selfNode = SearchNearAreaNode(selfPosition);
		auto targetNode = SearchNearAreaNode(targetPosition);

		SearchAstarStart(selfNode, targetNode, m_areaIndexGraph);

		return GetRouteAreaIndex();
	}

	//�T�[�`�{��
	void GraphAstar::SearchAstarStart(
		const std::shared_ptr<NavGraphNode>& selfNearNode, 
		const std::shared_ptr<NavGraphNode>& targetNearNode,
		const std::shared_ptr<GraphType>& graph
	) {
		if (selfNearNode == nullptr || targetNearNode == nullptr) {
			Debug::GetInstance()->Log(L"GraphAstar::SearchAstarStart(), node��null�ł�");
			return;
		}

		DebugStartAndEndIndexDraw(selfNearNode, targetNearNode);	//�J�n�m�[�h�ƏI���m�[�h�̃f�o�b�O�\��

		ResetAstar();	//�J�n�O�̏�������

		if (selfNearNode->GetPosition() == targetNearNode->GetPosition()) {	//�����m�[�h�Ȃ�true
			m_route.push(selfNearNode);
			return;
		}

		auto openDataHandler = OpenDataHandler();	//�����p�̃I�[�v���f�[�^�������N���X���쐬�B
		openDataHandler.StartSearchAstar(selfNearNode, targetNearNode, graph);	//OpenData���g���čŒZ�o�H����������B

		m_route = openDataHandler.GetRoute();		//���[�g�̎擾
	}

	void GraphAstar::SettingGraphMapCenterPositions() {
		for (auto pair : m_graphMap) {
			auto index = pair.first;
			auto graph = pair.second;
			auto centerPosition = graph->CalculateCenterPosition();	//���S�ʒu��ݒ肷��B

			//�{���͐�p�̕ʊ֐��ōs�����������B
			m_areaIndexGraph->AddNode(std::make_shared<NavGraphNode>(index, centerPosition, maru::ImpactData(index)));	//�G���A�C���f�b�N�X�O���t�ɂ��ǉ��B
		}

		AddEdges(m_areaIndexGraph);	//��L�Ɠ������ʂ̊֐��̏����ōs�����������B
	}

	int GraphAstar::SearchNearAreaIndex(const Vec3& position) {
		return SearchNearAreaNode(position)->GetAreaIndex();
	}

	std::shared_ptr<NavGraphNode> GraphAstar::SearchNearAreaNode(const Vec3& position) {
		std::shared_ptr<NavGraphNode> result = nullptr;

		float minRange = FLT_MAX;
		for (auto node : m_areaIndexGraph->GetNodes()) {
			auto toVec = position - node->GetPosition();
			auto range = toVec.length();
			if (range < minRange) {
				minRange = range;
				result = node;
			}
		}

		return result;
	}

	std::shared_ptr<GraphAstar::GraphType> GraphAstar::CreateCopyGraph() {
		return CreateCopyGraph(m_baseGraph);
	}

	//�����I�Ɋg��(Node�̃|�C���^�Q�Ƃ�؂�ւ������B)
	std::shared_ptr<GraphAstar::GraphType> GraphAstar::CreateCopyGraph(const std::shared_ptr<GraphType>& baseGraph) {
		auto graph = std::make_shared<GraphType>(true);

		graph->SetNodes(baseGraph->GetNodesCopy());
		graph->SetEdgesMap(baseGraph->GetEdgesMapCopy());

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
			auto top = copyRoute.top();
			
			resultPositions.push_back(top->GetPosition());
			Debug::GetInstance()->Log(top->GetIndex());
			
			copyRoute.pop();
		}

		return resultPositions;
	}

	std::vector<int> GraphAstar::GetRouteAreaIndex() const {
		std::vector<int> resultIndices;
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			auto top = copyRoute.top();

			resultIndices.push_back(top->GetAreaIndex());

			copyRoute.pop();
		}

		return resultIndices;
	}

	void GraphAstar::AddAreaGraphNode(const std::shared_ptr<NavGraphNode>& node) {
		//�G���A�C���f�b�N�X���Ƃ̃}�b�v�ɓo�^����B
		int areaIndex = node->GetAreaIndex();

		//�o�^�ς݂Ȃ�A���̒l�����炢�A�o�^����Ă��Ȃ��Ȃ�A�o�^���邵�Ēǉ�����B
		auto areaGraph = HasGraph(areaIndex) ? m_graphMap[areaIndex] : m_graphMap[areaIndex] = std::make_shared<GraphType>(true);
		areaGraph->AddNode(node);
	}

	int GraphAstar::AddNode(const Vec3& position,
		const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
	{
		//�m�[�h�̐���
		auto index = m_baseGraph->GetNextFreeNodeIndex();
		auto newNode = make_shared<NavGraphNode>(index, position);

		auto edges = UtilityAstar::CreateAdjacendEdges<NavGraphNode, AstarEdge>(m_baseGraph, newNode, obstacleObjs, excluteObjs);

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
		AddEdges(m_baseGraph);

		for (auto& pair : m_graphMap) {
			AddEdges(pair.second);
		}
	}

	void GraphAstar::AddEdges(const std::shared_ptr<GraphAstar::GraphType>& graph) {
		maru::Action<void()> actions;

		auto nodes = graph->GetNodes();
		for (auto& node : nodes) {
			auto edges = UtilityAstar::CreateAdjacendEdges<NavGraphNode, AstarEdge>(graph, node);
			//�G�b�W������Ȃ�������m�[�h���폜
			if (edges.size() == 0) {
				//�m�[�h�̍폜���܂Ƃ߂�B
				actions.AddFunction([&, this]() {graph->RemoveNode(node->GetIndex()); });
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
		maru::Utility::StackClear(m_route);
	}

	void GraphAstar::ResetAllAstar() {
		ResetAstar();
		m_baseGraph->ResetAll();
	}

	//�f�o�b�O����---------------------------------------------------------------------------------------------

	void GraphAstar::DebugNodeIndexDraw(const std::shared_ptr<NavGraphNode>& node, const std::wstring& description) {
		std::wstring indexStr = std::to_wstring(node->GetIndex());
		Debug::GetInstance()->Log(description + indexStr);
	}

	void GraphAstar::DebugStartAndEndIndexDraw(const std::shared_ptr<NavGraphNode>& startNode, const std::shared_ptr<NavGraphNode>& targetNode) {
		Debug::GetInstance()->Log(L"--------------------");

		DebugNodeIndexDraw(startNode,  L"StartNode : ");
		DebugNodeIndexDraw(targetNode, L"TargetNode: ");

		Debug::GetInstance()->Log(L"--------------------");
	}

	void GraphAstar::DebugRouteIndexDraw() {
		//��������Route��\������B
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			std::wstring debugStr = std::to_wstring(copyRoute.top()->GetIndex());
			debugStr += L",";

			Debug::GetInstance()->Log(debugStr);
			//DebugObject::AddValue(copyRoute.top()->GetIndex(), false);
			//DebugObject::AddString(L",", false);
			copyRoute.pop();
		}
	}

}

//endbasecross