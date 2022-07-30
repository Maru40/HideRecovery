/*!
@file BigShootSlime_Attack.h
@brief BigShootSlime_Attack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace BigShootSlime {
			namespace Task {
				struct Preliminary_Attack_Parametor;
				struct BodyBlow_Parametor;
			}
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	����V���[�g�X���C���̍U���p�����[�^
			//--------------------------------------------------------------------------------------
			struct BigShootSlime_Attack_Parametor {
				BigShootSlime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			///	����V���[�g�X���C���̍U���X�e�[�g
			//--------------------------------------------------------------------------------------
			class BigShootSlime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigShootSlime_Attack_Parametor;

				//--------------------------------------------------------------------------------------
				///	�^�X�N�^�C�v
				//--------------------------------------------------------------------------------------
				enum class TaskEnum {
					Preliminary, //�\������
					BodyBlow,    //BodyBlow
					Wait,        //�U����̍d������
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;               //�p�����[�^�B

				std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;  //�^�X�N���X�g�N���X�B

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="paramPtr">�p�����[�^</param>
				BigShootSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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

				/// <summary>
				/// �I�����f
				/// </summary>
				/// <returns>�X�V�I�������𖞂�������true</returns>
				bool IsEnd() const;

			};

		}
	}
}