
/*!
@file Stator_Hero.h
@brief Stator_Hero�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

#include "EnemyStatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		template<class NodeType, class EnumType, class TransitionMember> class StatorBase;

		//--------------------------------------------------------------------------------------
		///	�q�[���[�̃X�e�[�^�[�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct Stator_Hero_TransitionMember
		{
			float attackRange;	//�U������

			Stator_Hero_TransitionMember();
		};

		//--------------------------------------------------------------------------------------
		/// �q�[���[�̃X�e�[�^�[�̃u���b�N�{�[�h
		//--------------------------------------------------------------------------------------
		struct Stator_Hero_BlackBoard
		{
			
		};

		//--------------------------------------------------------------------------------------
		/// �q�[���[�̃X�e�[�^�[
		//--------------------------------------------------------------------------------------
		class Stator_Hero : 
			public EnemyStatorBase<EnemyBase, Stator_Hero_TransitionMember>,
			public I_BrackBoard<Stator_Hero_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_Hero_TransitionMember;
			using BlackBoard = Stator_Hero_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Stator_Hero(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڏ��������o�[</param>
			Stator_Hero(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

		private:
			void CreateNode() override;
			void CreateEdge() override;

			//--------------------------------------------------------------------------------------
			/// �p�����[�^�ݒ�
			//--------------------------------------------------------------------------------------

			void SettingParametor();
			void SettingPlowling();
		};

	}
}