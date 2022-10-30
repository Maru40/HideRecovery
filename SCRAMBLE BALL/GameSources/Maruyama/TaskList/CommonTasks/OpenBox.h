﻿/*!
@file OpenBox.h
@brief OpenBoxなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	class TargetManager;
	class HidePlaceOpener;
	class AccessHidePlace;

	template<class EnumType>
	class TaskList;

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	ボックスを開くタスク
		//--------------------------------------------------------------------------------------
		class OpenBox : public TaskNodeBase<GameObject>
		{
			std::weak_ptr<TargetManager> m_targetManager;
			std::weak_ptr<HidePlaceOpener> m_hidePlaceOpener;

		public:
			OpenBox(const std::shared_ptr<GameObject>& owner);

			void OnStart();

			bool OnUpdate();

			void OnExit();

		private:
			void Open();

			void ChangeAnimation();
		};

	}
}