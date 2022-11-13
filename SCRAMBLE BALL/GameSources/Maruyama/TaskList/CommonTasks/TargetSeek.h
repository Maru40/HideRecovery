/*!
@file TargetChase.h
@brief TargetChase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	class TargetManager;

	template<class EnumType>
	class TaskList;

	namespace Task {
		struct ToTargetMove_Parametor;
	}

	namespace TaskListNode {

		enum class TaskEnum {
			Move,
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g��ǂ�������^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct TargetSeek_Parametor
		{
			std::shared_ptr<Task::ToTargetMove_Parametor> toTargetMoveParam;

			TargetSeek_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g��ǂ�������^�X�N
		//--------------------------------------------------------------------------------------
		class TargetSeek : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = TargetSeek_Parametor;

		private:
			const Parametor* m_paramPtr;	//�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

			std::weak_ptr<Transform> m_transform;
			std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�

		public:
			TargetSeek(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr);

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:
			void SettingSeek();

			void UpdateSeekPosition();

			void DefineTask();

			void SelectTask();

		public:
			bool IsEnd() const;

		};

	}

}