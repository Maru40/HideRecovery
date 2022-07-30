/*!
@file Task_Preliminary_NormalSlime.h
@brief Task_Preliminary_NormalSlime�Ȃ�
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

		//--------------------------------------------------------------------------------------
		/// �\������^�X�N
		//--------------------------------------------------------------------------------------
		class Task_Preliminary_NormalSlime : public TaskNodeBase<EnemyBase>
		{
		public:
			/// <summary>
			/// �p�����[�^
			/// </summary>
			struct Parametor
			{
				float time;   //�ǂ̂��炢�̎��ԂŎ��k���邩
				float range;  //�ǂ̂��炢���k���邩

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="time">���k�ɂ����鎞��</param>
				/// <param name="range">���k���鋗��</param>
				Parametor(const float& time, const float& range);
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr; //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			/// <param name="paramPtr">�p�����[�^</param>
			Task_Preliminary_NormalSlime(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �����̍X�V
			/// </summary>
			void Rotation();
		};

	}
}