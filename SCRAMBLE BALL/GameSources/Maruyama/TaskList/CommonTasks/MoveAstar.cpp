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

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスクパラメータ
		//--------------------------------------------------------------------------------------

		MoveAstar_Parametor::MoveAstar_Parametor() :
			movePositionsParam(new Task_MovePositions_Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスク
		//--------------------------------------------------------------------------------------

		MoveAstar::MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const std::function<Parametor()>& getBlackBoardFunc) :
			TaskNodeBase_WithBlackBoard(owner, getBlackBoardFunc),
			//m_param(parametor),
			m_taskList(new TaskList<TaskEnum>())
		{
			InitializeParametor();
			DefineTask();

			m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
			m_targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>();
		}

		void MoveAstar::OnStart() {
			auto param = GetBlackBoard();
			SelectTask();	//タスクの選択

			CalculateMoveAreaRouteQueue();	//徘徊エリアルートの取得
			param.movePositionsParam->positions = CalculateMovePositions();	//徘徊移動先を設定

			Debug::GetInstance()->Log(L"SearchStart");
		}

		bool MoveAstar::OnUpdate() {
			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				NextRoute();
			}

			return IsEnd();
		}

		void MoveAstar::OnExit() {
			m_taskList->ForceStop();

			Debug::GetInstance()->Log(L"SearchEnd");
		}

		void MoveAstar::DefineTask() {
			auto param = GetBlackBoard();
			auto ownerObject = GetOwner()->GetGameObject();

			m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task_MovePositions>(ownerObject, param.movePositionsParam));
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
			if (m_areaRoute.empty()) {
				return;
			}

			auto& param = GetRefBlackBoard();
			param.movePositionsParam->positions = CalculateMovePositions();	//新しいポジションに変更

			SelectTask();				//タスクの再始動
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

			int areaIndex = m_areaRoute.front();
			m_areaRoute.pop();
			return maru::FieldImpactMap::GetInstance()->GetRoutePositions(startPosition, endPosition, areaIndex);
		}

		Vec3 MoveAstar::CalculateMoveTargetPosition() {
			auto targetManager = m_targetManager.lock();
			//必要コンポーネントが存在しないなら
			if (!targetManager || !targetManager->HasTarget()) {
				Debug::GetInstance()->Log(L"MoveAstar::CalculateMoveTargetPosition() で必要コンポーネントが存在しません。");

				//デバッグでHideを対象にする。
				auto hidePlace = maru::Utility::FindComponent<HidePlace>();
				auto position = hidePlace->GetGameObject()->GetComponent<Transform>()->GetPosition();
				return position;
				return Vec3(0.0f);
			}

			return targetManager->GetTargetPosition();	//基本ターゲット管理からの取得で検索できるようにする。
		}

		void MoveAstar::InitializeParametor() {
			auto& param = GetRefBlackBoard();

			param.movePositionsParam->moveParamPtr->speed = 10.0f;
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		bool MoveAstar::IsEnd() const {
			return m_areaRoute.empty() && m_taskList->IsEnd();
		}

	}
}