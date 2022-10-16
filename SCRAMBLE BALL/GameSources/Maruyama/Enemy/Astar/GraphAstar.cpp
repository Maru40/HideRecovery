/*!
@file GraphAstar.cpp
@brief GraphAstarのクラス実体
担当：丸山裕喜
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
	///	オープンデータ
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
	///	GraphAstarの本体
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

		//オブジェクトが探索ノードの近くにいたら、次のノードに切り替える。
		float nearRange = 2.0f;  //近くと判断される距離

		auto transform = objPtr->GetComponent<Transform>();
		auto objPosition = transform->GetPosition();

		auto nodePosition = m_route.top()->GetPosition();  //現在の目標ノードのポジションを取得

		auto toNode = nodePosition - objPosition;
		if (toNode.length() <= nearRange) {  //ノードの近くまで来たら。
			m_route.pop();
		}

		//ノードインデックスがショートを超えたら
		if (m_route.size() == 0) {
			m_isRouteEnd = true;//機能をoffにする。
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
			DebugObject::AddString(L"GraphAstar::CalculateRandomRoute(), GraphNodeが存在しません。");
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
			DebugObject::AddString(L"GraphAstar::SearchAstarStart(), nodeがnullです");
			return;
		}

		ResetAstar();

		m_heuristic->SetTargetNode(targetNearNode);  //ヒューリスティック関数に目標ノードを設定

		if (selfNearNode->GetPosition() == targetNearNode->GetPosition()) {
			m_route.push(selfNearNode);
			return;
		}

		//ループして処理を行う。
		LoopSearchAstar(selfNearNode, targetNearNode);
	}

	void GraphAstar::LoopSearchAstar(const std::shared_ptr<NavGraphNode>& initialNode, const std::shared_ptr<NavGraphNode>& targetNode) {
		int tempIndex = 0;
		int maxTempIndex = 1000;
		//グラフの中身コピー
		auto graph = CreateCopyGraph();
		m_openDataMap.clear();
		m_debugIndices.clear();
		m_openDataMap[initialNode->GetIndex()] = OpenData(initialNode, 0, m_heuristic->CalculateHeuristicRange(targetNode));
		//強制終了のバグなくなったら消す。
		while (tempIndex < maxTempIndex) {
			//オープンデータ生成用の基準ノードの生成。
			auto baseNode = CalculateCreateOpenDataBaseNode(initialNode);
			m_debugIndices.push_back(baseNode->GetIndex());
			//オープンデータの生成。ターゲットノードにたどり着いたらtrueを返す。
			if (CreateOpenData(baseNode, graph)) {
				break;
			}

			tempIndex++;
		}

		//オープンデータから最短経路を取得
		m_route.push(targetNode);
		m_openDataMap[targetNode->GetIndex()].isActive = false;
		CreateRoute(initialNode, targetNode, graph);

		//デバッグ---------------------------------------------

		//ループの上限を超えたことを伝える。
		//if (tempIndex >= maxTempIndex) {
		//	DebugObject::sm_wss << endl << L"serchOver" << endl;
		//	m_debugIndices;
		//}

		//DebugDraw(); //生成したRouteを表示する。
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

			//値が小さい場合
			if (pair.second.GetSumRange() < resultData.GetSumRange()) {
				resultData = pair.second;  //Result候補に入れる。
			}
		}

		return resultData.node.GetShard() == nullptr ? initialNode : resultData.node.GetShard();
	}

	bool GraphAstar::CreateOpenData(const ex_weak_ptr<NavGraphNode>& baseNode, const std::shared_ptr<GraphType>& graph) {
		//bool isArriveTargetNode = false;  //ターゲットノードにたどり着いたかどうか
		auto edges = graph->GetEdges(baseNode->GetIndex());	//エッジの取得

		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());
			if (!node->IsActive()) {
				continue;
			}

			auto toNodeVec = baseNode->GetPosition() - node->GetPosition();	//ノードへのベクトルを取得
			auto range = toNodeVec.length();
			auto heuristic = m_heuristic->CalculateHeuristicRange(node);	//ヒュースリック距離の取得

			auto newData = OpenData(node, range, heuristic);	//新規オープンデータの生成
			m_openDataMap[node->GetIndex()] = newData;

			//heuristicが限りなく小さかったらターゲットにたどり着いたため、終了。
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
		//ノードの中で一番近い物を取得
		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());
			//ノードが存在しない、または、データが非アクティブなら、処理を飛ばす。
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

		//初期ノードなら
		if (resultData->node == initialNode) {
			return; //処理をやめる。
		}

		m_route.push(resultData->node.GetShard());
		resultData->isActive = false;
		delete savePtr;

		CreateRoute(initialNode, resultData->node.GetShard(), graph);
	}

	//将来的に拡張(Nodeのポインタ参照を切り替えたい。)
	std::shared_ptr<GraphAstar::GraphType> GraphAstar::CreateCopyGraph() {
		auto graph = std::make_shared<GraphType>(true);

		int index = 0;
		for (auto& baseNode : m_baseGraph->GetNodes()) {
			graph->SetNodes(m_baseGraph->GetNodesCopy());
		}

		graph->SetEdgesMap(m_baseGraph->GetEdgesMapCopy());
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
			resultPositions.push_back(copyRoute.top()->GetPosition());
			copyRoute.pop();
		}

		return resultPositions;
	}

	//int GraphAstar::AddNode(const Vec3& position) {
	//	return AddNode(position);
	//	////ノードの生成
	//	//auto index = m_baseGraph->GetNextFreeNodeIndex();
	//	//auto newNode = std::make_shared<NavGraphNode>(index, position);

	//	//m_baseGraph->AddNode(newNode);

	//	//return index++;
	//}

	int GraphAstar::AddNode(const Vec3& position,
		const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
	{
		//ノードの生成
		auto index = m_baseGraph->GetNextFreeNodeIndex();
		auto newNode = make_shared<NavGraphNode>(index, position);

		auto edges = UtilityAstar::CreateAdjacendEdges(m_baseGraph, newNode, obstacleObjs, excluteObjs);

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
		maru::Action<void()> actions;

		auto nodes = m_baseGraph->GetNodes();
		for (auto& node : nodes) {
			auto edges = UtilityAstar::CreateAdjacendEdges<NavGraphNode, AstarEdge>(m_baseGraph, node);
			//エッジが一つもなかったらノードを削除
			if (edges.size() == 0) {
				//ノードの削除をまとめる。
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


	//デバッグ処理---------------------------------------------------------------------------------------------

	void GraphAstar::DebugDraw() {
		//生成したRouteを表示する。
		auto copyRoute = m_route;
		while (!copyRoute.empty()) {
			DebugObject::AddValue(copyRoute.top()->GetIndex(), false);
			DebugObject::AddString(L",", false);
			copyRoute.pop();
		}
	}

}

//endbasecross