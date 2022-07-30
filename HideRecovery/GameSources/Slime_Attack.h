/*!
@file Slime_Attack.h
@brief Slime_Attack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Task_Preliminary_NormalSlime.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	namespace Enemy {

		class EnemyBase;

		class Task_BodyBlow;
		struct Task_BodyBlow::Parametor;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �U���p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Attack_Parametor {
				std::shared_ptr<Task_Preliminary_NormalSlime::Parametor> preliminaryParamPtr = nullptr;    //�\������p�����[�^
				std::shared_ptr<Task_BodyBlow::Parametor> bodyBlowParamPtr = nullptr;                      //�{�f�B�u���[�p�����[�^
				std::shared_ptr<Task::Wait::Parametor> waitParamPtr = nullptr;                             //�ҋ@�p�����[�^

				Slime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			/// �U���X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Attack_Parametor;

				/// <summary>
				/// �^�X�N�^�C�v
				/// </summary>
				enum class TaskEnum {
					Preliminary, //�\������
					BodyBlow,    //�̓�����
					Wait,        //�U����̍d��
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr;               //�p�����[�^

				std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="parametor">�p�����[�^</param>
				Slime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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

			public: 
				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// ���݂̃^�X�N�^�C�v�̎擾
				/// </summary>
				TaskEnum GetCurrentTask() const;

			};

		}
	}
}