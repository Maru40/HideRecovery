
/*!
@file Task_AstarEvade.h
@brief Task_AstarEvade�Ȃ�
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

		//--------------------------------------------------------------------------------------
		/// Astar�𗘗p���������p�����[�^
		//--------------------------------------------------------------------------------------
		struct AstarEvade_Parametor {

		};

		//--------------------------------------------------------------------------------------
		/// Astar�𗘗p���������^�X�N
		//--------------------------------------------------------------------------------------
		class AstarEvade : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AstarEvade_Parametor;

			/// <summary>
			/// �^�X�N�^�C�v
			/// </summary>
			enum class TaskEnum {

			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr = nullptr;     //�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����N���X</param>
			/// <param name="paramPtr">�p�����[�^</param>
			AstarEvade(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}