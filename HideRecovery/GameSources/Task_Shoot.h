
/*!
@file Task_Shoot.h
@brief Task_Shoot�Ȃ�
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
				/// �V���b�g�^�X�N�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct Shoot_Parametor 
				{
					float speed;      //�e�������x
					float backPower;  //�����̗�

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					Shoot_Parametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="speed">�e�������x</param>
					/// <param name="backPower">�����̗�</param>
					Shoot_Parametor(const float& speed, const float& backPower);
				};

				//--------------------------------------------------------------------------------------
				/// �V���b�g�^�X�N
				//--------------------------------------------------------------------------------------
				class Shoot : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Shoot_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //�p�����[�^

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					/// <param name="paramPtr">�p�����[�^</param>
					Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

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