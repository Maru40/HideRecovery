
/*!
@file Task_Preliminary_Shoot.h
@brief Task_Preliminary_Shoot�Ȃ�
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

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// �V���b�g�U���̗\������p�����[�^
				//--------------------------------------------------------------------------------------
				struct Preliminary_Shoot_Parametor
				{
					std::shared_ptr<ShootChargeData> chargeDataPtr; //�`���[�W�f�[�^

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					Preliminary_Shoot_Parametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="chargeDataPtr">�`���[�W�f�[�^</param>
					Preliminary_Shoot_Parametor(const std::shared_ptr<ShootChargeData>& chargeDataPtr);
				};

				//--------------------------------------------------------------------------------------
				/// �V���b�g�U���̗\������^�X�N
				//--------------------------------------------------------------------------------------
				class Preliminary_Shoot : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Preliminary_Shoot_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //�p�����[�^�[

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					/// <param name="paramPtr">�p�����[�^</param>
					Preliminary_Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// �����̍X�V����
					/// </summary>
					void Rotation();

				public:
					//--------------------------------------------------------------------------------------
					/// �A�N�Z�b�T
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// �I�����f
					/// </summary>
					/// <returns>�X�V�������I��������true</returns>
					bool IsEnd();
				};

			}
		}
	}
}