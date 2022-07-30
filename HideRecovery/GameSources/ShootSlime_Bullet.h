/*!
@file ShootSlime_Bullet.h
@brief ShootSlime_Bullet�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̒e�p�����[�^
			//--------------------------------------------------------------------------------------
			struct Bullet_Parametor
			{
				DamageData damageData; //�_���[�W�f�[�^
				float destroyTime;     //�e���Ă��������܂ł̎���

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Bullet_Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="damageData">�_���[�W�f�[�^</param>
				/// <param name="destroyTime">�e���Ă��������܂ł̎���</param>
				Bullet_Parametor(const DamageData& damageData, const float& destroyTime);
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̒e
			//--------------------------------------------------------------------------------------
			class Bullet : public basecross::BulletBase
			{
			public:
				using Parametor = Bullet_Parametor;

			private:
				Parametor m_param; //�p�����[�^
				bool m_isActive;   //�A�N�e�B�u���ǂ���

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				Bullet(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="parametor">�p�����[�^</param>
				Bullet(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

				void OnCollisionEnter(const CollisionPair& pair) override;

			private:
				/// <summary>
				/// �����蔻�������Ԃ��ǂ���
				/// </summary>
				/// <param name="pair">������������</param>
				/// <returns>�����蔻������Ȃ�true</returns>
				bool IsCollision(const CollisionPair& pair);

			public:

				/// <summary>
				/// ������
				/// </summary>
				/// <param name="speed">�ړ��X�s�[�h</param>
				/// <param name="direct">�ړ�����</param>
				void Shoot(const float& speed, const Vec3& direct);

				/// <summary>
				/// ���鎞�̏���
				/// </summary>
				void DestroyProcess();
			};

		}

	}
}