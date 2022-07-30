/*!
@file Task_BodyBlow_BigSlime.h
@brief Task_BodyBlow_BigSlime�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "TaskList.h"

namespace basecross {

	namespace Enemy {
		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// ����X���C���̃{�f�B�u���[�̃p�����[�^�[
				//--------------------------------------------------------------------------------------
				struct BodyBlow_Parametor {
					Task_BodyBlow::Parametor sumAbsorbedParametor; //Absorbed�̃p�����[�^�̍��v���J�E���g

					BodyBlow_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// ����X���C���̃{�f�B�u���[�^�X�N
				//--------------------------------------------------------------------------------------
				class BodyBlow : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = BodyBlow_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //�p�����[�^
					bool m_isActive = false;            //���̃m�[�h�̃A�N�e�B�u��Ԃ�\���B

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
					/// <param name="paramPtr">�p�����[�^</param>
					BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// �ړ��X�V
					/// </summary>
					void MoveUpdate();

					/// <summary>
					/// �����X�V
					/// </summary>
					void Rotation();

					/// <summary>
					/// �ړ������������v������
					/// </summary>
					void ElapsedMoveRangeUpdate();

					/// <summary>
					/// ���������Ƃ��̃_���\�W��^����Ȃǂ̏���
					/// </summary>
					/// <param name="pair">���������f�[�^</param>
					void CollisionEnter(const CollisionPair& pair);

					/// <summary>
					/// �_���[�W���𓖂���������ɑ���
					/// </summary>
					/// <param name="pair"></param>
					void SendDamage(const CollisionPair& pair);

					/// <summary>
					/// �I�����f
					/// </summary>
					/// <returns>�X�V�I�������Ȃ�true</returns>
					bool IsEnd();
				};

			}
		}
	}
}