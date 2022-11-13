/*!
@file TargetChase.h
@brief TargetChaseなど
担当：丸山裕喜
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
		///	ターゲットを追いかけるタスクのパラメータ
		//--------------------------------------------------------------------------------------
		struct TargetSeek_Parametor
		{
			std::shared_ptr<Task::ToTargetMove_Parametor> toTargetMoveParam;

			TargetSeek_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	ターゲットを追いかけるタスク
		//--------------------------------------------------------------------------------------
		class TargetSeek : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = TargetSeek_Parametor;

		private:
			const Parametor* m_paramPtr;	//パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

			std::weak_ptr<Transform> m_transform;
			std::weak_ptr<TargetManager> m_targetManager;	//ターゲット管理

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