
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMoveのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetSeek.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	ターゲットを追いかけるタスクのパラメータ
		//--------------------------------------------------------------------------------------

		TargetSeek_Parametor::TargetSeek_Parametor():
			toTargetMoveParam(new Task::ToTargetMove::Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	ターゲットを追いかけるタスク
		//--------------------------------------------------------------------------------------

		TargetSeek::TargetSeek(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_paramPtr(paramPtr),
			m_taskList(new TaskList<TaskEnum>())
		{
			DefineTask();

			m_transform = owner->GetComponent<Transform>();
			m_targetManager = owner->GetComponent<TargetManager>();
		}

		void TargetSeek::OnStart() {
			//ターゲットが存在しないなら処理を飛ばす。
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {	
				return;
			}

			SettingSeek();	//追従開始前のセッティング

			SelectTask();
		}

		bool TargetSeek::OnUpdate() {
			UpdateSeekPosition();
			m_taskList->UpdateTask();

			return IsEnd();
		}

		void TargetSeek::OnExit() {
			m_taskList->ForceStop();
		}

		void TargetSeek::SettingSeek() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return;
			}

			auto& toTargetMoveParam = m_paramPtr->toTargetMoveParam;

			toTargetMoveParam->startPosition = GetOwner()->GetComponent<Transform>()->GetPosition();	//開始位置設定
			toTargetMoveParam->endPosition = targetManager->GetTargetPosition();						//終了位置設定
		}

		void TargetSeek::UpdateSeekPosition() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return;
			}

			auto& toTargetMoveParam = m_paramPtr->toTargetMoveParam;
			toTargetMoveParam->endPosition = targetManager->GetTargetPosition();	//終了位置設定
		}

		void TargetSeek::DefineTask() {
			auto owner = GetOwner();

			m_taskList->DefineTask(
				TaskEnum::Move,
				std::make_shared<Task::ToTargetMove>(owner, m_paramPtr->toTargetMoveParam)
			);
		}

		void TargetSeek::SelectTask() {
			TaskEnum tasks[] = {
				TaskEnum::Move,
			};

			for (auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		bool TargetSeek::IsEnd() const {
			//ターゲットが存在しないなら処理を飛ばす。
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return true;
			}

			//障害物に当たったら。
			auto objects = GetOwner()->GetStage()->GetGameObjectVec();
			if(maru::UtilityObstacle::IsRayObstacle(m_transform.lock()->GetPosition(), m_targetManager.lock()->GetTargetPosition(), objects)) {
				return true;
			}

			return m_taskList->IsEnd();
		}
	}
}

