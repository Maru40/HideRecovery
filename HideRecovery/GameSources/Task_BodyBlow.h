/*!
@file Task_BodyBlow.h
@brief Task_BodyBlow�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class TargetManager;

	struct DamageData;

	namespace Enemy {
		class EnemyBase;

		//--------------------------------------------------------------------------------------
		/// �{�f�B�[�u���[�^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct Task_BodyBlow_Parametor
		{
			float speed;                        //�ړ��X�s�[�h
			float range;                        //�ړ�����
			float endVelocitySpeed;             //�^�b�N����ɂǂ̌����X�s�[�h�ŏI�����邩
			float elapsedRange = 0.0f;          //�v������
			unique_ptr<DamageData> damageData;  //�_���[�W�f�[�^
			float toughness;                    //���x�x

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Task_BodyBlow_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="value">���ׂĂ̒l�̏������l</param>
			Task_BodyBlow_Parametor(const float& value);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ��X�s�[�h</param>
			/// <param name="range">�ړ�����</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ��X�s�[�h</param>
			/// <param name="range">�ړ�����</param>
			/// <param name="damageData">�_���[�W�f�[�^</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range, const DamageData& damageData);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ��X�s�[�h</param>
			/// <param name="range">�ړ�����</param>
			/// <param name="endVelocitySpeed">�^�b�N���I�����̌����X�s�[�h</param>
			/// <param name="damageData">�_���[�W�f�[�^</param>
			/// <param name="toughness">���x�x</param>
			Task_BodyBlow_Parametor(const float& speed, const float& range, const float& endVelocitySpeed, const DamageData& damageData, const float& toughness);

			/// <summary>
			/// �ړ��������ړ������������ǂ���
			/// </summary>
			/// <returns>�ړ�����������true</returns>
			bool IsMoveEnd();
		};

		//--------------------------------------------------------------------------------------
		/// �{�f�B�[�u���[�^�X�N
		//--------------------------------------------------------------------------------------
		class Task_BodyBlow : public TaskNodeBase<EnemyBase>
		{
		public:
			using Parametor = Task_BodyBlow_Parametor;

		private:
			bool m_isActive;                   //�A�N�e�B�u���ǂ���
			ex_weak_ptr<Parametor> m_paramPtr; //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			/// <param name="paramPtr">�p�����[�^</param>
			Task_BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �ړ��X�V
			/// </summary>
			void MoveUpdate();

			/// <summary>
			/// ��]�X�V
			/// </summary>
			void RotationUpdate();

			/// <summary>
			/// �ړ������������v������
			/// </summary>
			void ElapsedMoveRangeUpdate();  

			/// <summary>
			/// ���������Ƃ��̃_���\�W��^����Ȃǂ̏���
			/// </summary>
			/// <param name="pair">������������</param>
			void CollisionEnter(const CollisionPair& pair);

			/// <summary>
			/// ������������Ƀ_���[�W��`����
			/// </summary>
			/// <param name="pair">������������</param>
			void SendDamage(const CollisionPair& pair);

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �I�����f
			/// </summary>
			/// <returns>�X�V���I��������true</returns>
			bool IsEnd();
		};

	}
}