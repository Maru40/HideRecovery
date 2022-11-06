/*!
@file BehaviorTask_Shot.h
@brief BehaviorTask_Shotなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class UseWeapon;
	class TargetManager;
	class VelocityManager;
	class RotationController;
	class OnlinePlayerSynchronizer;

	template<class T>
	class TaskList;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace Task {

	}

	namespace TaskListNode {
		struct CrabWalk_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	移動タスクタイプ
				//--------------------------------------------------------------------------------------
				enum class MoveTaskEnum {
					CrabWalk,	//かに歩き
				};

				//--------------------------------------------------------------------------------------
				///	撃つ攻撃処理パラメータ
				//--------------------------------------------------------------------------------------
				struct Shot_Parametor {
					TaskListNode::CrabWalk_Parametor* crabWalkParamPtr;

					Shot_Parametor();

					virtual ~Shot_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	撃つ攻撃処理
				//--------------------------------------------------------------------------------------
				class Shot : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = Shot_Parametor;

				private:
					Parametor m_param;	//パラメータ

					std::weak_ptr<UseWeapon> m_useWeapon;							//武器の使用
					std::weak_ptr<TargetManager> m_targetManager;					//ターゲット管理
					std::weak_ptr<VelocityManager> m_velocityManager;				//速度管理
					std::weak_ptr<RotationController> m_rotationController;			//回転管理
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//オンラインシンクロ

					std::unique_ptr<TaskList<MoveTaskEnum>>	m_moveTaskList;			//移動用タスク

				public:
					Shot(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~Shot() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					void MoveUpdate();

					void RotationUpdate();

					void ShotUpdate();

					void DefineMoveTask();

					void SelectMoveTask();
				};

			}
		}
	}
}