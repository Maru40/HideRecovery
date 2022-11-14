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

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Enemy/ImpactMap/SelfImpactNodeManager.h"

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

			//減速処理開始
			//m_velocityManager.lock()->StartDeseleration();

			auto positions = CalculateMovePositions();	//新しいポジションに変更

			SelectTask();	//タスクの再始動

			//減速処理終了
			//m_velocityManager.lock()->SetIsDeseleration(false);
			SetIsSearchRoute(false);	//検索終了
		}

		std::queue<int> MoveAstar::CalculateMoveAreaRouteQueue() {
			maru::Utility::QueueClear(m_areaRoute);	//キューのクリア

			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();	//フィールド影響マップの取得

			//開始位置と目標位置の取得
			auto startPosition = m_transform.lock()->GetPosition();
			auto targetPosition = CalculateMoveTargetPosition();

			//エリアのルートを取得
			auto areaRouteIndices = fieldImpactMap->SearchAreaRouteIndices(startPosition, targetPosition);

			//Astar検索が最初の自分自身のノードを省くため、最初は現在所属しているエリアから検索する。
			m_areaRoute.push(fieldImpactMap->SearchNearAreaIndex(startPosition));
			for (const auto& areaRouteIndex : areaRouteIndices) {
				m_areaRoute.push(areaRouteIndex);
			}

			return m_areaRoute;
		}

		std::vector<Vec3> MoveAstar::CalculateMovePositions() {
			if (m_areaRoute.empty()) {
				return std::vector<Vec3>();
			}

			auto startPosition = m_transform.lock()->GetPosition();
			auto endPosition = CalculateMoveTargetPosition();

			int areaIndex = m_areaRoute.front();	//自分自身がいるエリアインデックス
			m_areaRoute.pop();
			int targetAreaIndex = !m_areaRoute.empty() ? m_areaRoute.front() : areaIndex;
			auto startNode = m_selfAstarNodeController.lock()->CalculateNode();
			//auto targetNode = 
			auto positions = maru::FieldImpactMap::GetInstance()->GetRoutePositions(startNode, CalculateMoveTargetNode(), areaIndex, targetAreaIndex);

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

			auto selfAstarNodeController = targetManager->GetTarget()->GetComponent<SelfAstarNodeController>(false);
			if (!selfAstarNodeController) {
				return nullptr;
			}

			auto node = selfAstarNodeController->CalculateNode();
			if (!node) {
				Debug::GetInstance()->Log(L"TaregetNodeがnullptrです。");
			}
			
			return node;
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