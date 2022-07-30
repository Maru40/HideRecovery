
/*!
@file SlimeStatorBase.h
@brief SlimeStatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//------------------------------------------------------------------------------------------------
		/// �X���C���̃X�e�[�g�^�C�v
		//------------------------------------------------------------------------------------------------
		enum class SlimeStateType {
			None,            //�������Ȃ����
			Start,           //�J�n�C�x���g���̏��
			Plowling,        //�p�j
			Find,            //����
			Chase,           //�Ǐ]
			Evade,           //������
			KnockBack,       //�m�b�N�o�b�N
			Attack,          //�U��
			Absorbed,        //�z��
			AbsorbedAttack,  //�z����Ԃ̍U��
			Pressed,         //�ׂ����
			Dyning,          //���S���o���
			Death,           //���S�Ɏ��S
		};

		//------------------------------------------------------------------------------------------------
		/// �X���C���X�e�[�^�[�̋K��N���X
		//------------------------------------------------------------------------------------------------
		template<class NodeType, class TransitionMember>
		class SlimeStatorBase : public StatorBase<NodeType, SlimeStateType, TransitionMember>
		{
		public:
			using State = SlimeStateType;

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SlimeStatorBase(const std::shared_ptr<GameObject>& objPtr)
				: SlimeStatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڏ����p�����o�[</param>
			SlimeStatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: StatorBase(objPtr, member)
			{}

		};
	}
}