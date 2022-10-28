/*!
@file I_Selecter.h
@brief I_Selecterなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	namespace maru {

		namespace Behavior {

			class I_Node;
			class I_Selecter;
			class Selecter;

			namespace SelecterTask {

				//使用禁止
				class Priority : public TaskNodeBase<Selecter>
				{
					std::weak_ptr<I_Node> m_currentNode;	//カレントノード

				public:
					Priority(const std::shared_ptr<Selecter>& selecter);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				public:
					bool HasCurrentNode() const;
				};

			}
		}
	}
}