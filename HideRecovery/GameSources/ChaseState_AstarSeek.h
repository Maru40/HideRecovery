/*!
@file ChaseState_AstarSeek.h
@brief ChaseState_AstarSeek�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class EnemyStateNodeBase;

	template<class EnumType>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Task {
		struct AroundEyeCheck_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			namespace ChaseState {

				//--------------------------------------------------------------------------------------
				/// Astar���g�����Ǐ]�p�����[�^
				//--------------------------------------------------------------------------------------
				struct AstarSeekParametor
				{
					std::shared_ptr<Task_MovePositions_Parametor> moveParamPtr;              //�ړ��p�����[�^
					std::shared_ptr<Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr;  //���͂��m�F����Ƃ��̃p�����[�^

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					AstarSeekParametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="moveParamPtr">�ړ��p�����[�^</param>
					/// <param name="aroundEyeCheckParamPtr">���͂��m�F����p�����[�^</param>
					AstarSeekParametor(const std::shared_ptr<Task_MovePositions_Parametor> moveParamPtr,
						const std::shared_ptr<Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// Astar���g�����Ǐ]�X�e�[�g
				//--------------------------------------------------------------------------------------
				class ChaseState_AstarSeek : public EnemyStateNodeBase<EnemyBase>
				{
				public:

					/// <summary>
					/// �^�X�N�^�C�v
					/// </summary>
					enum class TaskEnum {
						Move,            //�ړ�
						AroundMoveCheck, //���͂��ړ����Ċm�F
						AroundEyeCheck,  //���͂�ڂŊm�F
					};

				private:
					ex_weak_ptr<AstarSeekParametor> m_paramPtr = nullptr;      //�p�����[�^

					std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;  //�^�X�N���X�g�Ǘ�

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					/// <param name="paramPtr">�p�����[�^</param>
					ChaseState_AstarSeek(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<AstarSeekParametor>& paramPtr);

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
					/// �ړ��J�n����
					/// </summary>
					void ReserveMoveStart();

					/// <summary>
					/// ��]����
					/// </summary>
					void Rotation();
					/// <summary>
					/// �I�����̏���
					/// </summary>
					void EndProcess(); 

				public:

					//--------------------------------------------------------------------------------------
					/// �A�N�Z�b�T
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// �ړ��I�����f
					/// </summary>
					/// <returns>�ړI�m�[�h�ɋ߂Â��A�ړ����I��������true</returns>
					bool IsMoveEnd();
				};
			}
		}
	}
}