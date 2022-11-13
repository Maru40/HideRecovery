/*!
@file ToBallRunTask.h
@brief ToBallRunTaskなど
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
	class RotationController;
	class EyeSearchRange;
	class OnlinePlayerSynchronizer;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
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

				//--------------------------------------------------------------------------------------
				///	ボールへ向かうタスクのパラメータ
				//--------------------------------------------------------------------------------------
				struct ToBallRunTask_Parametor {
					basecross::Task::MoveAstar_Parametor* moveAstarParam;
					TaskListNode::TargetSeek_Parametor* targetSeekParam;
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParam;

					ToBallRunTask_Parametor();

					virtual ~ToBallRunTask_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	ボールへ向かうタスク
				//--------------------------------------------------------------------------------------
				class ToBallRunTask : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = ToBallRunTask_Parametor;

					enum class TaskEnum {
						MoveAstar,
						MoveArrive,
						GetBall,
						//Wait,
					};

				private:
					Parametor m_param;								//パラメータ

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

					std::weak_ptr<Transform> m_transform;							//トランスフォーム
					std::weak_ptr<EyeSearchRange> m_eyeRange;						//視界管理
					std::weak_ptr<TargetManager> m_targetManager;					//ターゲット管理
					std::weak_ptr<VelocityManager> m_velocityManager;				//速度管理
					std::weak_ptr<RotationController> m_rotationController;			//向き管理
					std::weak_ptr<Enemy::I_FactionMember> m_factionMember;			//ファクションメンバー
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//オンラインシンクロ

				public:
					ToBallRunTask(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~ToBallRunTask() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:

					/// <summary>
					/// ターゲットの取得
					/// </summary>
					/// <returns>ターゲット</returns>
					std::shared_ptr<GameObject> CalculateTarget();

					/// <summary>
					/// 次のMoveArriveタスクに変更するかを監視する。
					/// </summary>
					void CheckForceNextMoveArriveTask();

					/// <summary>
					/// タスクの定義
					/// </summary>
					void DefineTask();

					/// <summary>
					/// 待機中のタスクの定義
					/// </summary>
					void DefineWaitTask();

					/// <summary>
					/// タスクの選択
					/// </summary>
					void SelectTask();

					/// <summary>
					/// パラメータの初期化
					/// </summary>
					void InitializeParametor();

					/// <summary>
					/// 向き調整
					/// </summary>
					void Rotation();

				public:

					bool IsEnd() const;
				};

			}
		}
	}
}