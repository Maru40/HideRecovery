/*!
@file MoveAstar.cpp
@brief MoveAstarなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MoveAstar.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Enemy/ImpactMap/SelfImpactNodeManager.h"
#include "Maruyama/Enemy/Astar/OpenDataHandler.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Task {

		//std::mutex MoveAstar::m_mtx;

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスクパラメータ
		//--------------------------------------------------------------------------------------

		MoveAstar_Parametor::MoveAstar_Parametor() :
			movePositionsParam(new Task_MovePositions_Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスク
		//--------------------------------------------------------------------------------------


		MoveAstar::MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_param(paramPtr),
			m_taskList(new TaskList<TaskEnum>()),
			m_areaOpenDataHandler(new OpenDataHandler()),
			m_openDataHandler(new OpenDataHandler()),
			m_isInitializeSearch(true),
			m_isSearchRoute(false)
		{
			DefineTask();

			m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
			m_targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>();
			m_velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>();
			m_selfAstarNodeController = GetOwner()->GetGameObject()->GetComponent<SelfAstarNodeController>(false);
		}

		void MoveAstar::OnStart() {
			CalculateMoveAreaRouteQueue();	//徘徊エリアルートの取得

			//検索中なら処理を中断
			if (IsSearchRoute()) {
				return;
			}

			if (m_isInitializeSearch) {
				//初回検索のみバグるため、問題解決までの仮処理
				m_isInitializeSearch = false;
				NextRoute();
			}
			else {
				SetIsSearchRoute(true);
				//NextRoute();
				//スレッド生成
				std::thread nextRoute([&]() { NextRoute(); });
				nextRoute.detach();
			}
		}

		bool MoveAstar::OnUpdate() {
			//ルート検索中は他の処理を止める。
			if (IsSearchRoute()) {
				return false;
			}

			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				SetIsSearchRoute(true);
				//NextRoute();
				//スレッド生成
				std::thread nextRoute([&]() { NextRoute(); });
				nextRoute.detach();
			}

			return IsEnd();
		}

		void MoveAstar::OnExit() {
			m_taskList->ForceStop();
		}

		void MoveAstar::DefineTask() {
			auto ownerObject = GetOwner()->GetGameObject();

			m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task_MovePositions>(ownerObject, m_param->movePositionsParam));
		}

		void MoveAstar::SelectTask() {
			TaskEnum tasks[] = {
				TaskEnum::Move,
			};

			for (const auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		void MoveAstar::NextRoute() {
			//SetIsSearchRoute(true);		//検索開始
			//std::lock_guard<std::mutex> lock(m_mtx);

			if (m_areaRoute.empty()) {
				SetIsSearchRoute(false);//検索終了
				return;
			}

			//std::lock_guard<std::mutex> lock(m_mtx);
			auto positions = CalculateMovePositions();	//新しいポジションに変更

			SelectTask();	//タスクの再始動

			SetIsSearchRoute(false);	//検索終了
		}

		std::queue<int> MoveAstar::CalculateMoveAreaRouteQueue() {
			maru::Utility::QueueClear(m_areaRoute);	//キューのクリア

			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();	//フィールド影響マップの取得
			auto astar = fieldImpactMap->GetImpactMap()->GetGraphAstar();

			//開始位置と目標位置の取得
			auto startPosition = m_transform.lock()->GetPosition();
			auto targetPosition = CalculateMoveTargetPosition();

			//エリアのルートを取得
			auto areaRouteIndices = SearchAreaIndices(startPosition, targetPosition);

			//std::wstring debugStr = L"";	//デバッグ
			//Astar検索が最初の自分自身のノードを省くため、最初は現在所属しているエリアから検索する。
			int startAreaIndex = astar->SearchNearAreaIndex(startPosition);
			//debugStr += std::to_wstring(startAreaIndex);
			m_areaRoute.push(startAreaIndex);
			for (const auto& areaRouteIndex : areaRouteIndices) {
				//debugStr += L"," + std::to_wstring(areaRouteIndex);
				m_areaRoute.push(areaRouteIndex);
			}

			//Debug::GetInstance()->ClearLog();

			//Debug::GetInstance()->Log(L"エリアルート---------------------------");
			////std::reverse(debugStr.begin(), debugStr.end());
			//Debug::GetInstance()->Log(debugStr);
			//Debug::GetInstance()->Log(L"");

			return m_areaRoute;
		}

		std::vector<Vec3> MoveAstar::CalculateMovePositions() {
			if (m_areaRoute.empty()) {
				return std::vector<Vec3>();
			}

			int areaIndex = m_areaRoute.front();	//自分自身がいるエリアインデックス
			m_areaRoute.pop();
			int targetAreaIndex = !m_areaRoute.empty() ? m_areaRoute.front() : areaIndex;
			auto startNode = m_selfAstarNodeController.lock()->CalculateNode();

			auto positions = CalculateRoutePositions(startNode, CalculateMoveTargetNode(), areaIndex, targetAreaIndex);

			m_param->movePositionsParam->positions = positions;
			return positions;
		}

		Vec3 MoveAstar::CalculateMoveTargetPosition() {
			auto targetManager = m_targetManager.lock();
			//必要コンポーネントが存在しないなら
			if (!targetManager || !targetManager->HasTarget()) {
				//Debug::GetInstance()->Log(L"MoveAstar::CalculateMoveTargetPosition() で必要コンポーネントが存在しません。");
				return Vec3(0.0f);
			}

			return targetManager->GetTargetPosition();	//基本ターゲット管理からの取得で検索できるようにする。
		}

		std::shared_ptr<NavGraphNode> MoveAstar::CalculateMoveTargetNode() {
			auto targetManager = m_targetManager.lock();
			//必要コンポーネントが存在しないなら
			if (!targetManager || !targetManager->HasTarget()) {
				return nullptr;
			}

			auto target = targetManager->GetTarget();
			auto selfAstarNodeController = targetManager->GetTarget()->GetComponent<SelfAstarNodeController>(false);
			if (!selfAstarNodeController) {
				return nullptr;
			}

			auto node = selfAstarNodeController->CalculateNode();
			if (!node) {
				Debug::GetInstance()->Log(target->GetComponent<Transform>(false)->GetPosition());
				Debug::GetInstance()->Log(L"TaregetNodeがnullptrです。");
			}
			
			return node;
		}

		std::vector<int> MoveAstar::SearchAreaIndices(const Vec3& startPosition, const Vec3& targetPosition) {
			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();
			auto impactMap = fieldImpactMap->GetImpactMap();
			auto astar = impactMap->GetGraphAstar();
			
			auto areaGraph = astar->GetAreaGraph();

			auto selfNode = astar->SearchNearAreaNode(startPosition);
			auto targetNode = astar->SearchNearAreaNode(targetPosition);

			auto route = SearchAstarStart(selfNode, targetNode, areaGraph, m_areaOpenDataHandler);

			std::vector<int> resultIndices;
			while (!route.empty()) {
				auto top = route.top();
				if (top.expired()) {
					route.pop();
					continue;
				}

				resultIndices.push_back(top.lock()->GetAreaIndex());

				route.pop();
			}

			return resultIndices;
		}

		std::vector<Vec3> MoveAstar::CalculateRoutePositions(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int areaIndex,
			const int targetAreaIndex
		) {
			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();
			auto impactMap = fieldImpactMap->GetImpactMap();
			auto astar = impactMap->GetGraphAstar();

			auto graph = astar->GetBaseGraph();

			auto route = SearchAstarStart(selfNode, targetNode, graph, m_openDataHandler, targetAreaIndex);

			std::vector<Vec3> resultPositions;

			while (!route.empty()) {
				auto top = route.top();
				if (top.expired()) {
					route.pop();
					continue;
				}

				resultPositions.push_back(top.lock()->GetPosition());

				route.pop();
			}

			return resultPositions;
		}

		std::stack<std::weak_ptr<NavGraphNode>> MoveAstar::SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<AstarGraph>& graph,
			const std::shared_ptr<OpenDataHandler> openDataHandler,
			const int targetAreaIndex
		) {
			std::stack<std::weak_ptr<NavGraphNode>> result;

			if (selfNode == nullptr || targetNode == nullptr) {
				if (selfNode == nullptr) {
					Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), startNodeがnullです");
				}

				if (targetNode == nullptr) {
					Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), targetNodeがnullです");
				}

				return result;
			}

			if (selfNode->GetPosition() == targetNode->GetPosition()) {	//同じノードならtrue
				Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), 同じルートです。");
				result.push(selfNode);
				return result;
			}

			openDataHandler->StartSearchAstar(selfNode, targetNode, graph, targetAreaIndex);	//OpenDataを使って最短経路を検索する。

			return openDataHandler->GetRoute();		//ルートの取得
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		bool MoveAstar::IsEnd() const {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return true;
			}

			return m_areaRoute.empty() && m_taskList->IsEnd();
		}

	}
}