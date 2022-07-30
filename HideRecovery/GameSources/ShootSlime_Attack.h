
/*!
@file ShootSlime_Attack.h
@brief ShootSlime_Attack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Task{
		struct Wait_Parametor;
	}
	
	namespace Enemy {

		class EnemyBase;

		namespace ShootSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// �U���p�����[�^�[
				//--------------------------------------------------------------------------------------
				struct Attack_Parametor 
				{
					std::shared_ptr<Task::Preliminary_Shoot_Parametor> preliminaryParamPtr = nullptr; //�\������p�����[�^
					std::shared_ptr<Task::Shoot_Parametor> shootParamPtr = nullptr;                   //���p�����[�^
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;          //�d���p�����[�^

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					Attack_Parametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="preliminaryParamPtr">�\������p�����[�^</param>
					/// <param name="shootParamPtr">���p�����[�^</param>
					/// <param name="waitParamPtr">�d���p�����[�^</param>
					Attack_Parametor(
						const  std::shared_ptr<Task::Preliminary_Shoot_Parametor>& preliminaryParamPtr,
						const std::shared_ptr<Task::Shoot_Parametor>& shootParamPtr,
						const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// �V���b�g�X���C���̍U���X�e�[�g
				//--------------------------------------------------------------------------------------
				class Attack : public EnemyStateNodeBase<EnemyBase>
				{
				public:
					/// <summary>
					/// �^�X�N�^�C�v
					/// </summary>
					enum class TaskEnum {
						Preliminary,  //�\������
						Shoot,        //�U������
						Wait,         //�U����d��
						Reload,       //�e�̃����[�h
					};

					using Parametor = Attack_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;               //�p�����[�^

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					/// <param name="paramPtr">�p�����[�^</param>
					Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// �^�X�N�̒�`
					/// </summary>
					void DefineTask();

					/// <summary>
					/// �^�X�N�̑I��
					/// </summary>
					void SelectTask();

				};

			}
		}
	}
}