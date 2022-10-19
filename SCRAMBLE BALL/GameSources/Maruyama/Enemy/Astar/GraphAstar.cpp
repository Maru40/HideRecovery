/*!
@file GraphAstar.cpp
@brief GraphAstarのクラス実体
担当：丸山裕喜
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
	///	GraphAstarの本体
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
		bool isObstacleConfirmation = (areaIndex == targetAreaIndex);	//エリアが同じなら、障害物判定を行う。
		auto targetNearNode = UtilityAstar::SearchNearNode(graph, targetPosition, isObstacleConfirmation);

		SearchAstarStart(selfNearNode, targetNearNode, m_baseGraph);
	}

	std::vector<Vec3> GraphAstar::CalculateRandomRoute(const Vec3& selfPosition) {
		if (m_baseGraph->GetNodes().size() == 0) {
			Debug::GetInstance()->Log(L"GraphAstar::CalculateRandomRoute(), GraphNodeが存在しません。");
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

	//サーチ本体
	void GraphAstar::SearchAstarStart(
		const std::shared_ptr<NavGraphNode>& selfNearNode, 
		const std::shared_ptr<NavGraphNode>& targetNearNode,
		const std::shared_ptr<GraphType>& graph
	) {
		if (selfNearNode == nullptr || targetNearNode == nullptr) {
			Debug::GetInstance()->Log(L"GraphAstar::SearchAstarStart(), nodeがnullです");
			return;
		}

		DebugStartAndEndIndexDraw(selfNearNode, targetNearNode);	//開始ノードと終了ノードのデバッグ表示

		ResetAstar();	//開始前の初期準備

		if (selfNearNode->GetPosition() == targetNearNode->GetPosition()) {	//同じノードならtrue
			m_route.push(selfNearNode);
			return;
		}

		auto openDataHandler = OpenDataHandler();	//検索用のオープンデータを扱うクラスを作成。
		openDataHandler.StartSearchAstar(selfNearNode, targetNearNode, graph);	//OpenDataを使って最短経路を検索する。

		m_route = openDataHandler.GetRoute();		//ルートの取得
	}

	void GraphAstar::SettingGraphMapCenterPositions() {
		for (auto pair : m_graphMap) {
			auto index = pair.first;
			auto graph = pair.second;
			auto centerPosition = graph->CalculateCenterPosition();	//中心位置を設定する。

			//本来は専用の別関数で行いたい処理。
			m_areaIndexGraph->AddNode(std::make_shared<NavGraphNode>(index, centerPosition, maru::ImpactData(index)));	//エリアインデックスグラフにも追加。
		}

		AddEdges(m_areaIndexGraph);	//上記と同じく別の関数の処理で行いたい処理。
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

	//将来的に拡張(Nodeのポインタ参照を切り替えたい。)
	std::shared_ptr<GraphAstar::GraphType> GraphAstar::CreateCopyGraph(const std::shared_ptr<GraphType>& baseGraph) {
		auto graph = std::make_shared<GraphType>(true);

		graph->SetNodes(baseGraph->GetNodesCopy());
		graph->SetEdgesMap(baseGraph->GetEdgesMapCopy());

		return graph;
	}

	//アクセッサ------------------------------------------------------------------------------------------------------

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
		//エリアインデックスごとのマップに登録する。
		int areaIndex = node->GetAreaIndex();

		//登録済みなら、その値をもらい、登録されていないなら、登録するして追加する。
		auto areaGraph = HasGraph(areaIndex) ? m_graphMap[areaIndex] : m_graphMap[areaIndex] = std::make_shared<GraphType>(true);
		areaGraph->AddNode(node);
	}

	int GraphAstar::AddNode(const Vec3& position,
		const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
	{
		//ノードの生成
		auto index = m_baseGraph->GetNextFreeNodeIndex();
		auto newNode = make_shared<NavGraphNode>(index, position);

		auto edges = UtilityAstar::CreateAdjacendEdges<NavGraphNode, AstarEdge>(m_baseGraph, newNode, obstacleObjs, excluteObjs);

		//現在はエッジが作成されたときのみ生成するようにする。(繋がってないノードを対象にした場合に処理が止まってしまうため。)
		//改善され次第外す。
		if (edges.size() != 0) {
			//ノードの追加
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
			//エッジが一つもなかったらノードを削除
			if (edges.size() == 0) {
				//ノードの削除をまとめる。
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

	//デバッグ処理---------------------------------------------------------------------------------------------

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
		//生成したRouteを表示する。
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