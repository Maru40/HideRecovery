/*!
@file BehaviorTree.h
@brief BehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree;

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// �X�e�[�g�}�V���ɑg�ݍ��ނȂǂ́ASubBehavior�Ƃ��ė��p���鎞�̊��N���X
				//--------------------------------------------------------------------------------------
				template<class EnumType>
				class SubBehaviorTreeBase
				{
					std::weak_ptr<GameObject> m_owner;	//�I�[�i�[�^�C�v

				protected:
					
					std::unique_ptr<BehaviorTree<EnumType>> m_behaviorTree;	//�r�w�C�r�A�c���[

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