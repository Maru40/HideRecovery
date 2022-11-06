/*!
@file BehaviorTask_Shot.h
@brief BehaviorTask_Shot�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class UseWeapon;
	class TargetManager;
	class EyeSearchRange;
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
				///	�ړ��^�X�N�^�C�v
				//--------------------------------------------------------------------------------------
				enum class MoveTaskEnum {
					CrabWalk,	//���ɕ���
				};

				//--------------------------------------------------------------------------------------
				///	���U�������p�����[�^
				//--------------------------------------------------------------------------------------
				struct Shot_Parametor {
					TaskListNode::CrabWalk_Parametor* crabWalkParamPtr;

					Shot_Parametor();

					virtual ~Shot_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	���U������
				//--------------------------------------------------------------------------------------
				class Shot : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = Shot_Parametor;

				private:
					Parametor m_param;	//�p�����[�^

					std::weak_ptr<UseWeapon> m_useWeapon;							//����̎g�p
					std::weak_ptr<TargetManager> m_targetManager;					//�^�[�Q�b�g�Ǘ�
					std::weak_ptr<EyeSearchRange> m_eyeRange;						//���E�Ǘ�
					std::weak_ptr<VelocityManager> m_velocityManager;				//���x�Ǘ�
					std::weak_ptr<RotationController> m_rotationController;			//��]�Ǘ�
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//�I�����C���V���N��

					std::unique_ptr<TaskList<MoveTaskEnum>>	m_moveTaskList;			//�ړ��p�^�X�N

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

				public:

					bool IsEnd() const;
				};

			}
		}
	}
}