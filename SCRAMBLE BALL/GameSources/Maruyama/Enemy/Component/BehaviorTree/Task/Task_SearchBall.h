/*!
@file Task_SearchBall.h
@brief Task_SearchBallなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {
	
	template<class T>
	class TaskList;

	class TargetManager;
	class VelocityManager;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {
		struct MoveAstar_Parametor;
		struct ToTargetMove_Parametor;
		struct Wait_Parametor;
	}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				enum class TaskEnum {
					MoveAstar,			//Astarを利用してターゲットの近くまで移動する。
					ArriveParamSetting,	//ターゲットへの最後の移動のパラメータセッティング
					MoveArrive,			//ターゲットが視界内なら到着行動
										//アイテムを空ける。
					Wait,				//待機
				};

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスクパラメータ
				//--------------------------------------------------------------------------------------
				struct SearchBall_Parametor {
					basecross::Task::MoveAstar_Parametor* moveAstarParam;
					//std::shared_ptr<basecross::Task::ToTargetMove_Parametor> toTargetMoveParam;
					TaskListNode::TargetSeek_Parametor* targetSeekParam;
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParam;

					SearchBall_Parametor();

					virtual ~SearchBall_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスク
				//--------------------------------------------------------------------------------------
				class SearchBall : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = SearchBall_Parametor;

				private:
					Parametor m_param;								//パラメータ
					
					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

					std::weak_ptr<Transform> m_transform;
					std::weak_ptr<TargetManager> m_targetManager;
					std::weak_ptr<VelocityManager> m_velocityManager;

				public:
					SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:

					/// <summary>
					/// ターゲットの取得
					/// </summary>
					/// <returns>ターゲット</returns>
					std::shared_ptr<GameObject> CalculateTarget();

					void DefineTask();

					void SelectTask();

					/// <summary>
					/// パラメータの初期化
					/// </summary>
					void InitializeParametor();

				public:

					bool IsEnd() const;
				};

			}

		}
	}
}