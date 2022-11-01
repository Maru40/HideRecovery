/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTree�Ȃ�
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

		enum class HidePlacePatrolTree_BehaviorType {
			FirstSelecter,		//����m�[�h

			PatrolTask,			//�p�g���[���^�X�N
		};

		class HidePlacePatrolTree
		{
		public:
			using BehaviorType = HidePlacePatrolTree_BehaviorType;
			using BehaviorTree = maru::Behavior::BehaviorTree<BehaviorType>;

		private:
			std::weak_ptr<GameObject> m_owner;				//�I�[�i�[�I�u�W�F�N�g

			std::unique_ptr<BehaviorTree> m_behaviorTree;	//�r�w�C�r�A�c���[

		public:
			HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr);

			virtual ~HidePlacePatrolTree() = default;

			void OnCreate();
			void OnUpdate();

		protected:

			void CreateNode();
			void CreateEdge();
			void CreateDecorator();

		public:

			_NODISCARD std::shared_ptr<GameObject> GetOwner() const noexcept { return m_owner.lock(); }
		};

	}
}