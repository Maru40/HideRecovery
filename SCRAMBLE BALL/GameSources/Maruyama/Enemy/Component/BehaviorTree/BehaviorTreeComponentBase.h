/*!
@file BehaviorTreeComponentBase.h
@brief BehaviorTreeComponentBaseなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {
		namespace Behavior {
			template<class EnumType>
			class BehaviorTree;
		}
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// ビヘイビアツリーコンポーネントの基底クラス
		//--------------------------------------------------------------------------------------
		template<class EnumType>
		class BehaviorTreeComponentBase : public Component
		{
		protected:
			std::unique_ptr<maru::Behavior::BehaviorTree<EnumType>> m_behaviorTree;	//ビヘイビアツリー

			virtual void CreateNode() = 0;
			virtual void CreateEdge() = 0;

		public:
			BehaviorTreeComponentBase(const std::shared_ptr<GameObject>& objPtr) :
				Component(objPtr),
				m_behaviorTree(new maru::Behavior::BehaviorTree<EnumType>())
			{}

			void OnLateStart() override final {
				CreateNode();
				CreateEdge();
			}

			void OnUpdate() override {
				m_behaviorTree->OnUpdate();
			}

		};

	}
}