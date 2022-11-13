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
				if (!IsActive()) {	//非アクティブなら遷移できないため、false
					return false;
				}

				//完了状態なら遷移しない。
				if (IsState(BehaviorState::Completed)) {
					return false;
				}

				//デコレータがないなら、常にtrue
				if (IsDecoratorEmpty()) {	
					return true;
				}

				//一つでも遷移できないならfalse
				for (const auto& decorator : m_decorators) {
					decorator->OnStart();	//遷移条件を検索する前に初期化する。
					if (!decorator->CanTransition()) {
						return false;
					}
				}

				return true;	//全てのデコレータがtrueなら遷移できる。
			}

			bool NodeBase::CanUpdate() const {
				if (IsDecoratorEmpty()) {
					return true;
				}

				//一つでも更新できないならfalse
				for (const auto& decorator : m_decorators) {
					if (!decorator->CanUpdate()) {
						return false;
					}
				}

				return true;	//全てのデコレータがtrueなら更新できる。
			}

			void NodeBase::AddDecorator(const std::shared_ptr<I_Decorator>& decorator) {
				m_decorators.push_back(decorator);
			}

			std::vector<std::shared_ptr<I_Decorator>> NodeBase::GetDecorators() const {
				return m_decorators;
			}

			bool NodeBase::IsDecoratorEmpty() const {
				return static_cast<int>(m_decorators.size()) == 0;
			}

		}
	}
}