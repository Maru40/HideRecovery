/*!
@file BehaviorTreeComponentBase.h
@brief BehaviorTreeComponentBase�Ȃ�
�S���ҁF�ێR �T��
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
		/// �r�w�C�r�A�c���[�R���|�[�l���g�̊��N���X
		//--------------------------------------------------------------------------------------
		template<class EnumType>
		class BehaviorTreeComponentBase : public Component
		{
		protected:
			std::unique_ptr<maru::Behavior::BehaviorTree<EnumType>> m_behaviorTree;	//�r�w�C�r�A�c���[

			virtual void CreateNode() = 0;
			virtual void CreateEdge() = 0;
			virtual void CreateDecorator() = 0;

		public:
			BehaviorTreeComponentBase(const std::shared_ptr<GameObject>& objPtr) :
				Component(objPtr),
				m_behaviorTree(new maru::Behavior::BehaviorTree<EnumType>())
			{}

			void OnLateStart() override final {
				CreateNode();
				CreateEdge();
				CreateDecorator();
			}

			void OnUpdate() override {
				m_behaviorTree->OnUpdate();
			}

		};

	}
}