/*!
@file RelifMember.h
@brief RelifMemberなど
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
		struct AroundEyeCheck_Parametor;
	}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	見方の周りの警戒タスクパラメータ
				//--------------------------------------------------------------------------------------
				struct RelifMember_Parametor {
					basecross::Task::MoveAstar_Parametor* moveAstarParam;
					TaskListNode::TargetSeek_Parametor* targetSeekParam;
					std::shared_ptr<basecross::Task::AroundEyeCheck_Parametor> aroundEyeCheckParam;	//周囲警戒パラメータ
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParam;

					RelifMember_Parametor();

					virtual ~RelifMember_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	見方の周りの警戒タスク
				//--------------------------------------------------------------------------------------
				class RelifMember : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = RelifMember_Parametor;

					enum class TaskEnum {
						MoveAstar,			//Astarを利用してターゲットの近くまで移動する。
						MoveArrive,			//ターゲットが視界内なら到着行動
						AroundPtrol,		//周りを警戒
						Wait,
					};

				private:
					Parametor m_param;

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

					std::weak_ptr<Transform> m_transform;					//トランスフォーム
					std::weak_ptr<EyeSearchRange> m_eyeRange;				//視界管理
					std::weak_ptr<TargetManager> m_targetManager;			//ターゲット管理
					std::weak_ptr<VelocityManager> m_velocityManager;		//速度管理
					std::weak_ptr<RotationController> m_rotationController;	//向き管理
					std::weak_ptr<Enemy::I_FactionMember> m_factionMember;	//ファクションメンバー

				public:
					RelifMember(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~RelifMember() = default;

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