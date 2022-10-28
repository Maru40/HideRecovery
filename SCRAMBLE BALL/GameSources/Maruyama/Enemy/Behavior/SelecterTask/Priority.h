/*!
@file I_Selecter.h
@brief I_Selecter�Ȃ�
�S���ҁF�ێR �T��
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

				//�g�p�֎~
				class Priority : public TaskNodeBase<Selecter>
				{
					std::weak_ptr<I_Node> m_currentNode;	//�J�����g�m�[�h

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