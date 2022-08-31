
/*!
@file EnemyStatusBase.h
@brief EnemyStatusBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�̃X�e�[�^�X
		//--------------------------------------------------------------------------------------
		struct EnemyStatus
		{
			float hp;                    //�q�b�g�|�C���g
			float maxHp;                 //�ő�q�b�g�|�C���g
			float toughness;             //���x�x
			float baseToughness;         //�������x�x

			EnemyStatus();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="maxHp">�ő�HP</param>
			/// <param name="toughness">���x�x</param>
			EnemyStatus(const float& maxHp, const float& toughness);
		};

		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�̃X�e�[�^�X�Ǘ��̊��N���X
		//--------------------------------------------------------------------------------------
		class EnemyStatusManagerBase : public Component
		{
		protected:
			EnemyStatus m_status;  //�X�e�[�^�X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">�������g�����L����Q�[���I�u�W�F�N�g</param>
			EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">�������g�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="status">�X�e�[�^�X</param>
			EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status);

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �X�e�[�^�X�̃Z�b�g
			/// </summary>
			/// <param name="status">�X�e�[�^�X</param>
			void SetStatus(const EnemyStatus& status) noexcept;

			/// <summary>
			/// �X�e�[�^�X�̎擾
			/// </summary>
			/// <returns>�X�e�[�^�X</returns>
			EnemyStatus GetStatus() const noexcept;

			/// <summary>
			/// �X�e�[�^�X�̎Q�Ƃ��擾
			/// </summary>
			/// <returns>�X�e�[�^�X�̎Q��</returns>
			EnemyStatus& GetRefStatus();

			/// <summary>
			/// ���x�x�𒴂���͂��ǂ���
			/// </summary>
			/// <param name="power">�^������p���[</param>
			/// <returns>���x�x�𒴂�����true</returns>
			bool IsToughnessBreak(const float& power);
		};

	}
}