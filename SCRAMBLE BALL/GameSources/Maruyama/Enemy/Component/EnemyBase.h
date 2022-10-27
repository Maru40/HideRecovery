
/*!
@file EnemyBase.h
@brief EnemyBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

//#include "Watanabe/Utility/Utility.h"
//#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {

	class TargetManager;

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�̊��N���X
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember
		{
			//team::TeamType m_teamType;						//�`�[���^�C�v

			std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr) :
				Component(objPtr)
			{ }

			virtual void OnStart() override;

			//--------------------------------------------------------------------------------------
			///	�C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			virtual void SetTarget(const std::shared_ptr<GameObject>& target) override;
			 
			virtual std::shared_ptr<GameObject> GetTarget() const override;

			virtual std::shared_ptr<GameObject> GetSelfObject() const override;
		};
	}
}

//endbasecross