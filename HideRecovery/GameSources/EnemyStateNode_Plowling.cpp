
/*!
@file EnemyStateNode_Plowling.cpp
@brief EnemyStateNode_Plowlingのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "TargetManager.h"
#include "TaskList.h"
#include "Task_Wait.h"
#include "EnemyStateNodeBase.h"
#include "EnemyStateNode_Plowling.h"

#include "UtilityVelocity.h"
#include "VelocityManager.h"
#include "EnemyBase.h"

#include "Task_PlowlingMove.h"

#include "SingletonComponent.h"
#include "ImpactMap.h"
#include "GraphAstar.h"
#include "SparseGraph.h"
#include "UtilityAstar.h"

#include "FieldImpactMap.h"
#include "GameTimer.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			Plowling::Parametor::Parametor() :
				Parametor(std::make_shared<Task_PlowlingMove::Parametor>(), std::make_shared<Task::Wait::Parametor>())
			{}

			Plowling::Parametor::Parametor(
				const std::shared_ptr<Task_PlowlingMove::Parametor>& moveParamPtr,
				const std::shared_ptr<Task::Wait::Parametor>& waitParamPtr
			) :
				moveParamPtr(moveParamPtr),
				waitParamPtr(waitParamPtr)
			{}


			//--------------------------------------------------------------------------------------
			/// スライムの徘徊タスクタイプ
			//--------------------------------------------------------------------------------------

			enum class Plowling::TaskEnum
			{
				Move,  //移動
				Wait,  //待機(目的地についた時に一定時間待機)
			};

			//--------------------------------------------------------------------------------------
			/// スライムの徘徊行動ステート本体
			//--------------------------------------------------------------------------------------

			Plowling::Plowling(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
				EnemyStateNodeBase<EnemyBase>(owner), 
				m_paramPtr(paramPtr), 
				m_taskList(new TaskList<TaskEnum>()), 
				m_arriveTimer(new GameTimer(0.0f))
			{
				DefineTask();
			}

			void Plowling::OnStart() {
				CalculateAstarRamdomPosition();

				//ポジションが設定されていないなら処理をしない
				if (m_paramPtr->positions.size() == 0) { 
					DebugObject::AddString(L"Slime_Plowling::OnStartで、ポジションが設定されていません。");
					return;
				}

				//向かうポジションを設定
				m_index = 0;
				m_addIndexDirect = 1;
				ChangeMoveTargetPosition(m_paramPtr->positions[m_index]);

				m_taskList->ForceStop(false);
				SelectTask();
			}

			bool Plowling::OnUpdate() {
				//ポジションが設定されていないなら処理をしない
				if (m_paramPtr->positions.size() == 0) {  
					CalculateAstarRamdomPosition();
					return false; 
				}

				m_taskList->UpdateTask();

				//タスクが終了したら
				if (m_taskList->IsEnd()) { 
					NextPosition(); //次のポジションをセット
					SelectTask();   //タスクの再セット
					return false;
				}

				//到着時間計測
				m_arriveTimer->UpdateTimer();
				if (m_arriveTimer->IsTimeUp()) {
					//到着時刻でもたどり着かなかったら、経路再建策。
					OnStart();
				}

				return false;
			}

			void Plowling::OnExit() {
				m_taskList->ForceStop(false);

				//デバックカラーを解除
				ChangeDebugNodeColor(Col4(1.0f));
			}

			void Plowling::DefineTask() {
				auto enemy = GetOwner();

				//移動
				m_taskList->DefineTask(TaskEnum::Move, make_shared<Task_PlowlingMove>(enemy, m_paramPtr->moveParamPtr));

				//待機
				m_taskList->DefineTask(TaskEnum::Wait, make_shared<Task::Wait>(m_paramPtr->waitParamPtr));
			}

			void Plowling::SelectTask() {
				TaskEnum tasks[] = {
					TaskEnum::Move,
					TaskEnum::Wait
				};

				for (const auto& task : tasks) {
					m_taskList->AddTask(task);
				}
			}

			void Plowling::NextIndex() {
				m_index += m_addIndexDirect; //カウントを進める。

				if (m_index >= m_paramPtr->positions.size() || m_index < 0) {
					m_addIndexDirect *= -1;      //カウントを進める方向を逆にする。
					m_index += m_addIndexDirect; //カウントを進める。
				}
			}

			void Plowling::NextPosition() {
				//DebugDrawで元の色に戻す。
				ChangeDebugNodeColor(Col4(1.0f));

				NextIndex();
				Vec3 targetPosition = m_paramPtr->positions[m_index];
				ChangeMoveTargetPosition(targetPosition);  //移動場所を変更する。
			}

			void Plowling::ChangeMoveTargetPosition(const Vec3& position) {
				auto& moveParamPtr = m_paramPtr->moveParamPtr->moveParamPtr;
				moveParamPtr->startPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				moveParamPtr->endPosition = position;

				//到着予測時刻を決定する。
				m_arriveTimer->ResetTimer(CalculateArriveTime(position, moveParamPtr->speed));

				//DebugDrawで目的地の色を変更する。
				ChangeDebugNodeColor(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			void Plowling::CalculateAstarRamdomPosition() {
				//Astarからポジション群を取得
				auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();
				if (!impactMap) {
					DebugObject::AddString(L"ImpactMapが定義されていません。");
					return;
				}

				auto astar = impactMap->GetGraphAstar();
				auto positions = astar->CalculateRandomRoute(GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition());

				for (auto& position : positions) {
					position.y = 0.0f;
				}

				m_paramPtr->positions = positions;
			}

			float Plowling::CalculateArriveTime(const Vec3& targetPosition, const float& speed) const {
				auto selfPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				auto toTargetVec = targetPosition - selfPosition;
				constexpr float GraceTime = 1.0f;   //猶予時間

				//時間 = 距離 / 速度
				float time = toTargetVec.length() / speed;

				return time + GraceTime;
			}

			//デバッグ------------------------------------------------------------------------------------------------------------------

			void Plowling::ChangeDebugNodeColor(const Col4& color) {
				if (auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap()) {
					auto node = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), m_paramPtr->positions[m_index]);
					if (node) {
						//impactMap->ChangeTargetNodeColor(node->GetIndex(), color);
					}
				}
			}

		}
	}
}