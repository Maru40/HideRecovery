
/*!
@file Slime_KnockBack.h
@brief Slime_KnockBack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	template<class T>
	class EnemyStateNodeBase;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̃m�b�N�o�b�N�p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_KnockBack_Parametor
			{
				float maxPower = 10.0f;                                          //�m�b�N�o�b�N�̍ő�p���[
				std::function<void()> startAnimationFunction = nullptr;          //�X�^�[�g���ɐ؂�ւ������A�j���[�V����
				std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr;   //�d���p�����[�^

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Slime_KnockBack_Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="waitParamPtr">�d���p�����[�^</param>
				Slime_KnockBack_Parametor(const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
			};

			//--------------------------------------------------------------------------------------
			/// �X���C���̃m�b�N�o�b�N�X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_KnockBack : public StateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_KnockBack_Parametor;

				/// <summary>
				/// �^�X�N�^�C�v
				/// </summary>
				enum class TaskEnum {
					NotDeselerationWait,  //�������Ȃ����
					Wait,                 //�d��
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr;              //�p�����[�^

				std::unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N�Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Slime_KnockBack(const std::shared_ptr<EnemyBase>& owner ,const std::shared_ptr<Parametor>& paramPtr);

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