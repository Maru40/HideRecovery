
/*!
@file EvadeSlime_Evade.h
@brief EvadeSlime_Evade�Ȃ�
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

	namespace Task {
		struct AroundEyeCheck_Parametor;
		struct AstarMove_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace Task {
			struct Flee_Parametor;
			
		}

		namespace EvadeSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// ������X�e�[�g�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct Evade_Parametor
				{
					std::shared_ptr<Enemy::Task::Flee_Parametor> fleeParamPtr;                         //�Ώۂ���^�t�ɓ�����p�����[�^
					std::shared_ptr<basecross::Task::AstarMove_Parametor> astarMoveParamPtr;           //Astar�𗘗p���ē�����p�����[�^
					std::shared_ptr<basecross::Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr; //���͂��m�F����p�����[�^

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					Evade_Parametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="fleeParamPtr">�Ώۂ���^�t�ɓ�����p�����[�^</param>
					/// <param name="astarMoveParamPtr">Astar�𗘗p���ē�����p�����[�^</param>
					/// <param name="aroundEyeCheckParamPtr">���͂��m�F����p�����[�^</param>
					Evade_Parametor(
						const std::shared_ptr<Enemy::Task::Flee_Parametor>& fleeParamPtr,
						const std::shared_ptr<basecross::Task::AstarMove_Parametor>& astarMoveParamPtr,
						const std::shared_ptr<basecross::Task::AroundEyeCheck_Parametor>& aroundEyeCheckParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// ������X�e�[�g
				//--------------------------------------------------------------------------------------
				class Evade : public EnemyStateNodeBase<EnemyBase>
				{
				public:
					using Parametor = Evade_Parametor;

					/// <summary>
					/// �^�X�N�^�C�v
					/// </summary>
					enum class TaskEnum {
						Flee,            //�Ώۂ̐^�t�Ɉړ����铦���s��
						AstarEvade,      //Astar�𗘗p���������s��
						AroundEyeCheck,  //�����ڂŊm�F����B
					};

				private:
					ex_weak_ptr<Parametor> m_paramPtr;              //�p�����[�^

					std::unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N�Ǘ��N���X

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="objPtr">���̃N���X�����L����N���X</param>
					/// <param name="paramPtr">�p�����[�^</param>
					Evade(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor> paramPtr);

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