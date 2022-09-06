
/*!
@file EnemyStateNode_Plowling.h
@brief EnemyStateNode_Plowling�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/CommonTasks/Task_PlowlingMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class MulchTaskList;

	template<class T>
	class TaskList;

	template<class T>
	class EnemyStateNodeBase;
	
	class TargetManager;
	class GameTimer;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̜p�j�s���X�e�[�g
			//--------------------------------------------------------------------------------------
			class Plowling : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// �p�j�^�X�N�^�C�v
				/// </summary>
				enum class TaskEnum;

				//--------------------------------------------------------------------------------------
				/// �p�����[�^
				//--------------------------------------------------------------------------------------
				struct Parametor
				{
					vector<Vec3> positions;                                                   //�p�j����|�W�V�����z��
					std::shared_ptr<Task_PlowlingMove::Parametor> moveParamPtr = nullptr;     //�ړ�����p�����[�^
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;  //�ҋ@���̃p�����[�^

					Parametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="moveParamPtr">�ړ��p�����[�^</param>
					/// <param name="waitParamPtr">�ҋ@�p�����[�^</param>
					Parametor(const std::shared_ptr<Task_PlowlingMove::Parametor>& moveParamPtr, 
						const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
				};
				
			private:
				unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N���X�g���Ǘ�����N���X
				unique_ptr<GameTimer> m_arriveTimer;       //�������Ԃ��v������B(��莞�ԓ��ɓ����ł��Ă��Ȃ���΁A���[�g�č\�z)

				ex_weak_ptr<Parametor> m_paramPtr;         //�p�����[�^
				int m_index = 0;						   //���݂̃C���f�b�N�X
				int m_addIndexDirect = 1;				   //�C���f�b�N�X��i�߂����

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Plowling(const std::shared_ptr<EnemyBase>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

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
				/// ���̖ړI�n�̃C���f�b�N�X�ɕύX
				/// </summary>
				void NextIndex();

				/// <summary>
				/// ���̖ړI�n�ɕύX
				/// </summary>
				void NextPosition();

				/// <summary>
				/// �ړ����ύX
				/// </summary>
				/// <param name="position">�ύX�������ړ���</param>
				void ChangeMoveTargetPosition(const Vec3& position);

				/// <summary>
				/// Astar�𗘗p����RandomRoute���v��
				/// </summary>
				void CalculateAstarRamdomPosition();

				/// <summary>
				/// �����\�����Ԃ��v�Z���Ď擾
				/// </summary>
				/// <param name="targetPosition">�����n�_</param>
				/// <param name="speed">�ړ��X�s�[�h</param>
				/// <returns>�����\������</returns>
				float CalculateArriveTime(const Vec3& targetPosition, const float& speed) const;

				//�f�o�b�O-----------------------------------------------------------------------

				void ChangeDebugNodeColor(const Col4& color);
			};

		}
	}
}