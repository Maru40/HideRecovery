/*!
@file I_Node.cpp
@brief I_Nodeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

#include "I_Decorator.h"

namespace basecross {
	namespace maru {

		namespace Behavior {


			//--------------------------------------------------------------------------------------
			/// ビヘイビアタスクの基底クラス
			//--------------------------------------------------------------------------------------

			bool NodeBase::CanTransition() const {
				if (IsDecoratorEmpty()) {	//デコレータがないなら、常にtrue
					return true;
				}

				//一つでも遷移できないならfalse
				for (const auto& decorator : m_decorators) {
					if (!decorator->CanTransition()) {
						return false;
					}
				}

				return true;	//全てのデコレータがtrueなら遷移できる。
			}

			void NodeBase::AddDecorator(const std::shared_ptr<I_Decorator>& decorator) {
				m_decorators.push_back(decorator);
			}

			bool NodeBase::IsDecoratorEmpty() const {
				return static_cast<int>(m_decorators.size()) == 0;
			}

		}
	}
}