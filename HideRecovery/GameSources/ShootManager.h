
/*!
@file ShootManager.h
@brief ShootManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�Ǘ��̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct ShootManager_Parametor
			{

			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�`���[�W�f�[�^
			//--------------------------------------------------------------------------------------
			struct ShootChargeData 
			{
				float time;  //�`���[�W����
				float range; //�ړ�����

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				ShootChargeData();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="time">�`���[�W����</param>
				/// <param name="range">�ړ�����</param>
				ShootChargeData(const float& time, const float& range);
			};

			//--------------------------------------------------------------------------------------
			/// �V���b�g�Ǘ��N���X
			//--------------------------------------------------------------------------------------
			class ShootManager : public Component
			{
			public:
				using Parametor = ShootManager_Parametor;

			private:
				Parametor m_param;                      //�p�����[�^

				ex_weak_ptr<GameObject> m_bulletObject; //�o���b�g�I�u�W�F�N�g

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				ShootManager(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="parametor">�p�����[�^</param>
				ShootManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

				void OnCreate() override;
				void OnDestroy() override;

			private:
				/// <summary>
				/// �o���b�g�I�u�W�F�N�g�𐶐�����
				/// </summary>
				void CrealteBulletObject();

			public:
				/// <summary>
				/// ���S�ɏW�߂鏈�����J�n����B
				/// </summary>
				/// <param name="time">�`���[�W����</param>
				/// <param name="range">�ړ�����</param>
				void StartCharge(const float& time, const float& range);

				/// <summary>
				/// ���S�ɏW�߂鏈�����J�n����B
				/// </summary>
				/// <param name="data">�`���[�W�f�[�^</param>
				void StartCharge(const ShootChargeData& data);

				/// <summary>
				/// Shoot�𒆒f����Break���鏈��
				/// </summary>
				void Break();

				/// <summary>
				/// �`���[�W�̏I���𔻒f����B
				/// </summary>
				/// <returns>�`���[�W���I��������true</returns>
				bool IsEndCharge() const;

				/// <summary>
				/// ������
				/// </summary>
				/// <param name="speed">�ړ��X�s�[�h</param>
				/// <param name="target">���^�[�Q�b�g</param>
				void Shoot(const float& speed, const std::shared_ptr<GameObject>& target);

				/// <summary>
				/// ������
				/// </summary>
				/// <param name="speed">�ړ��X�s�[�h</param>
				/// <param name="targetPosition">�^�[�Q�b�g�̈ʒu</param>
				void Shoot(const float& speed, const Vec3& targetPosition);

				/// <summary>
				/// �����[�h
				/// </summary>
				void Reload();
			};

		}
	}
}