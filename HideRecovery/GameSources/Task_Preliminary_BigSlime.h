/*!
@file Task_Preliminary_BigSlime.h
@brief Task_Preliminary_BigSlime�Ȃ�
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

	namespace Enemy {
		class EnemyBase;

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// ����X���C���̍U���\������p�����[�^
				//--------------------------------------------------------------------------------------
				struct Preliminary_Attack_Parametor {
					float time;   //�ǂ̂��炢�̎��ԂŎ��k����

					Preliminary_Attack_Parametor();
					Preliminary_Attack_Parametor(const float& time);
				};

				//--------------------------------------------------------------------------------------
				/// ����X���C���̍U���\������^�X�N
				//--------------------------------------------------------------------------------------
				class Preliminary_Attack : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Preliminary_Attack_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //�p�����[�^

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
					/// <param name="paramPtr">�p�����[�^</param>
					Preliminary_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// �����̒�������
					/// </summary>
					void Rotation();
				};

			}
		}
	}
}

