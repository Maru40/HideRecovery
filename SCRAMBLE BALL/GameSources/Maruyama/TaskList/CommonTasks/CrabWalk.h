
/*!
@file CrabWalk.h
@brief CrabWalk�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class TaskList;

	class GameTimer;
	class VelocityManager;
	class PlayerControlManager;
	class OnlinePlayerSynchronizer;

	class UseWeapon;

	namespace Operator {
		class ObjectMover;
	}

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	���ɕ����^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct CrabWalk_Parametor {
			float speed;		//�ړ��X�s�[�h
			float minMoveTime;	//�ړ����Ԃ̍ŏ�����
			float maxMoveTime;	//�ړ����Ԃ̍ő厞��

			CrabWalk_Parametor();
			CrabWalk_Parametor(const float speed);
			CrabWalk_Parametor(const float speed, const float minMoveTime, const float maxMoveTime);
		};

		//--------------------------------------------------------------------------------------
		///	���ɕ����^�X�N
		//--------------------------------------------------------------------------------------
		class CrabWalk : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = CrabWalk_Parametor;

			enum class MoveDirectionType {
				None,
				Right,
				Left,
			};

		private:
			const Parametor* m_paramPtr;			//�p�����[�^

			MoveDirectionType m_directionType;		//�ړ�����
			std::unique_ptr<GameTimer> m_timer;		//�^�C�}�[

			std::weak_ptr<Transform> m_transform;					//�g�����X�t�H�[��
			std::weak_ptr<UseWeapon> m_useWepon;					//����̎g�p
			std::weak_ptr<Operator::ObjectMover> m_mover;			//�ړ��n
			std::weak_ptr<PlayerControlManager> m_playerController;	//player�R���g���[���[
			std::weak_ptr<VelocityManager> m_velocityManager;		//���x�Ǘ�
			std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSychoronizer;	//�I�����C���V���N��

		public:
			CrabWalk(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr);

			virtual ~CrabWalk() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void MoveUpdate();

			/// <summary>
			/// �����_���Ɉړ����������߂�
			/// </summary>
			/// <returns>�����_���Ɍ��߂�ꂽ�ړ�����</returns>
			MoveDirectionType GetRandomMoveDirectionType();

			/// <summary>
			/// �ړ�������Ԃ�
			/// </summary>
			/// <returns>�ړ�����</returns>
			Vec3 CalculateMoveDirection();

			bool IsEnd() const;
		};

	}
}