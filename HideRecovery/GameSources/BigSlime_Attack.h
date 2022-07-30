/*!
@file BigSlime_Attack.h
@brief BigSlime_Attack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	class GameTimer;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace BigSlime {
			namespace Task {
				struct Preliminary_Attack_Parametor;
				struct BodyBlow_Parametor;
			}
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// ����X���C���̍U���p�����[�^
			//--------------------------------------------------------------------------------------
			struct BigSlime_Attack_Parametor {

				std::shared_ptr<BigSlime::Task::Preliminary_Attack_Parametor> preliminaryParamPtr = nullptr;    //�\������p�����[�^
				std::shared_ptr<BigSlime::Task::BodyBlow_Parametor> bodyBlowParamPtr = nullptr;                 //�{�f�B�u���[�p�����[�^
				std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;                        //�ҋ@�p�����[�^

				BigSlime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			/// ����X���C���̍U���X�e�[�g
			//--------------------------------------------------------------------------------------
			class BigSlime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigSlime_Attack_Parametor;

				//--------------------------------------------------------------------------------------
				/// �^�X�N�^�C�v
				//--------------------------------------------------------------------------------------
				enum class TaskEnum {
					Preliminary, //�\������
					BodyBlow,    //�{�f�B�[�u���[
					Wait,        //�U����̍d��
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr;              //�p�����[�^

				std::unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N�Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="paramPtr">�p�����[�^</param>
				BigSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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