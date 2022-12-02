/*!
@file EnemyStatorBase.h
@brief EnemyStatorBase�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Stator/StatorBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	�X�e�[�g�^�C�v
		//--------------------------------------------------------------------------------------
		enum class StateType
		{
			None,            //�������Ȃ����
			Start,           //�J�n�C�x���g���̏��
			Plowling,        //�p�j
			Find,            //����
			Chase,           //�Ǐ]
			Evade,           //������
			KnockBack,       //�m�b�N�o�b�N
			Attack,          //�U��
			Pressed,         //�ׂ����
			Dyning,          //���S���o���
			Death,           //���S�Ɏ��S
		};

		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�X�e�[�^�[�̊��N���X
		//--------------------------------------------------------------------------------------
		template<class NodeType, class TransitionMember>
		class EnemyStatorBase : public StatorBase<NodeType, StateType, TransitionMember>
		{
		public:
			using State = StateType;

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			EnemyStatorBase(const std::shared_ptr<GameObject>& objPtr)
				: SlimeStatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڏ����p�����o�[</param>
			EnemyStatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: StatorBase(objPtr, member)
			{}
		};

	}
}