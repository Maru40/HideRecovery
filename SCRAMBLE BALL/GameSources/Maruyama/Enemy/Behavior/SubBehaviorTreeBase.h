/*!
@file BehaviorTree.h
@brief BehaviorTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// 前方宣言
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree;

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// ステートマシンに組み込むなどの、SubBehaviorとして利用する時の基底クラス
				//--------------------------------------------------------------------------------------
				template<class EnumType>
				class SubBehaviorTreeBase
				{
					std::weak_ptr<GameObject> m_owner;	//オーナータイプ

				protected:
					
					std::unique_ptr<BehaviorTree<EnumType>> m_behaviorTree;	//ビヘイビアツリー

				public:
					SubBehaviorTreeBase(const std::shared_ptr<GameObject>& owner) :
						m_owner(owner),
						m_behaviorTree(new BehaviorTree<EnumType>)
					{}

					virtual ~SubBehaviorTreeBase() = default;

					void OnCreate() final {
						CreateNode();
						CreateEdge();
						CreateDecorator();
					}

				protected:
					
					virtual void CreateNode() = 0;
					virtual void CreateEdge() = 0;
					virtual void CreateDecorator() = 0;

				public:

					_NODISCARD std::shared_ptr<GameObject> GetOwner() const noexcept { return m_owner.lock(); }

				};

			}
		}
	}
}