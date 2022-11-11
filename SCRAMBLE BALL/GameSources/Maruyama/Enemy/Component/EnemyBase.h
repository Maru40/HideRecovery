
/*!
@file EnemyBase.h
@brief EnemyBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

//#include "Watanabe/Utility/Utility.h"
//#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {

	class TargetManager;
	class OnlinePlayerSynchronizer;
	class VelocityManager;

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�̊��N���X
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember, public Tuple::I_Tupler
		{
			std::shared_ptr<Tuple::TupleSpace> m_tupleSpace;	//�^�v���X�y�[�X

			std::weak_ptr<TargetManager> m_targetManager;		//�^�[�Q�b�g�Ǘ�
			std::weak_ptr<VelocityManager> m_velocityManager;	//���x�Ǘ�
			std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//�I�����C���V���N��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr);

			virtual ~EnemyBase() = default;

			virtual void OnCreate() override;
			virtual void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	�C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			virtual void SetTarget(const std::shared_ptr<GameObject>& target) override;
			 
			virtual std::shared_ptr<GameObject> GetTarget() const override;

			virtual std::shared_ptr<GameObject> GetSelfObject() const override;

			_NODISCARD virtual std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept override;
		};
	}
}

//endbasecross