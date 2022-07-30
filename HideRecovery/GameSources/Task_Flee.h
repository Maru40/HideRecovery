
/*!
@file Task_Flee.h
@brief Task_Flee�Ȃ�
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

	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace Task {

			//--------------------------------------------------------------------------------------
			/// ���S�^�X�N�p�����[�^
			//--------------------------------------------------------------------------------------
			struct Flee_Parametor {
				float maxSpeed;       //�ő�X�s�[�h
				float time;           //�����鎞��
				bool isTimer = true;  //���Ԍo�߂ŏI�����邩�ǂ���

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Flee_Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="maxSpeed">�ő�X�s�[�h</param>
				Flee_Parametor(const float& maxSpeed);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="maxSpeed">�ő�X�s�[�h</param>
				/// <param name="time">�����鎞��</param>
				/// <param name="isTimer">���Ԍo�߂ŏI�����邩�ǂ���</param>
				Flee_Parametor(const float& maxSpeed, const float& time, const bool isTimer = true);
			};

			//--------------------------------------------------------------------------------------
			/// ���S�^�X�N
			//--------------------------------------------------------------------------------------
			class Flee : public TaskNodeBase<EnemyBase>
			{
			public:
				using Parametor = Flee_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //�p�����[�^

				std::unique_ptr<GameTimer> m_timer;  //�^�C���Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Flee(const std::shared_ptr<EnemyBase>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// �ړ��X�V
				/// </summary>
				void MoveUpdate();

				/// <summary>
				/// ��]�X�V
				/// </summary>
				void RotationUpdate();

			public:
				/// <summary>
				/// �I�����f
				/// </summary>
				/// <returns>���S������������true</returns>
				bool IsEnd();

			};

		}

	}
}