
/*!
@file Stator_Villain.h
@brief Stator_Villain�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Patch/Trigger.h"
#include "Maruyama/Interface/I_BrackBoard.h"

#include "EnemyStatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		template<class NodeType, class EnumType, class TransitionMember> class StatorBase;

		//--------------------------------------------------------------------------------------
		///	���B�����̃X�e�[�^�[�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct Stator_Villain_TransitionMember
		{
			float plowlingEyeRange;
			float attackRange;	//�U������

			Stator_Villain_TransitionMember();
		};

		//--------------------------------------------------------------------------------------
		/// ���B�����̃X�e�[�^�[�̃u���b�N�{�[�h
		//--------------------------------------------------------------------------------------
		struct Stator_Villain_BlackBoard
		{

		};

		//--------------------------------------------------------------------------------------
		/// ���B�����̃X�e�[�^�[
		//--------------------------------------------------------------------------------------
		class Stator_Villain :
			public EnemyStatorBase<EnemyBase, Stator_Villain_TransitionMember>,
			public I_BrackBoard<Stator_Villain_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_Villain_TransitionMember;
			using BlackBoard = Stator_Villain_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Stator_Villain(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڏ��������o�[</param>
			Stator_Villain(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

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