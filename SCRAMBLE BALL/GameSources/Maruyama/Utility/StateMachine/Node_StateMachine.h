/*!
@file Node_StateMachine.h
@brief Node_StateMachine�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "NodeBase.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �R���|�[�l���g�̕ύX�֌W�̏����܂Ƃ߂��\����
		//--------------------------------------------------------------------------------------
		struct ChangeComponentParametor
		{
			std::shared_ptr<Component> component; //�R���|�[�l���g�̃|�C���^
			bool isStart;                         //�J�n���ɂǂ����ɂ��邩�H
			bool isExit;                          //�I�����ɂǂ����ɂ��邩�H

			ChangeComponentParametor(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit
			) :
				component(component),
				isStart(isStart),
				isExit(isExit)
			{}
		};

		//--------------------------------------------------------------------------------------
		/// �X�e�[�g�}�V���̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_StateNode
		{
		public:
			virtual void OnStart() = 0;
			virtual bool OnUpdate() = 0;
			virtual void OnExit() = 0;

			virtual ~I_StateNode() = default;
		};

		//--------------------------------------------------------------------------------------
		/// �X�e�[�g�}�V���p�̃m�[�h���N���X
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class NodeBase_StateMachine : public NodeBase, public I_StateNode
		{
			std::weak_ptr<OwnerType> m_owner;	//�I�[�i�[

		public:
			NodeBase_StateMachine(const std::shared_ptr<OwnerType>& owner) :
				NodeBase_StateMachine(0, owner)
			{}

			NodeBase_StateMachine(const int index, const std::shared_ptr<OwnerType>& owner):
				NodeBase(index),
				m_owner(owner)
			{}

			virtual ~NodeBase_StateMachine() = default;

		public:

			/// <summary>
			/// ���L�҂̎擾
			/// </summary>
			/// <returns>���L��</returns>
			std::shared_ptr<OwnerType> GetOwner() const {
				auto owner = m_owner.lock();
				return owner ? owner : nullptr;
			}

		};

		//--------------------------------------------------------------------------------------
		/// �X�e�[�g�}�V���p�̃m�[�h���N���X�̊g��(�R���|�[�l���g�̐؂�ւ���ǉ�)
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class NodeBase_StateMachine_Ex : public NodeBase_StateMachine<OwnerType>
		{
			std::vector<ChangeComponentParametor> m_changeParams;	//�R���|�[�l���g�ύX�p�̃p�����[�^�z��

		public:
			NodeBase_StateMachine_Ex(const std::shared_ptr<OwnerType>& owner):
				NodeBase_StateMachine_Ex(0, owner)
			{}

			NodeBase_StateMachine_Ex(const int index, const std::shared_ptr<OwnerType>& owner):
				NodeBase_StateMachine<OwnerType>(index, owner)
			{}

			virtual ~NodeBase_StateMachine_Ex() = default;

		protected:

			/// <summary>
			/// �J�n�ƏI�����ɐ؂�ւ���R���|�[�l���g�̒ǉ�
			/// </summary>
			/// <param name="component">�؂�ւ���R���|�[�l���g�̃|�C���^</param>
			/// <param name="isStart">�X�^�[�g���ɂǂ����ɐ؂�ւ���</param>
			/// <param name="isExit">�I�����ɂǂ����ɐ؂�ւ��邩</param>
			void AddChangeComponent(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit)
			{
				if (component == nullptr) {  //nullptr�Ȃ�ǉ����Ȃ�
					return;
				}

				ChangeComponentParametor param(component, isStart, isExit);
				m_changeParams.push_back(param);
			}

			/// <summary>
			/// �J�n�ƏI�����ɐ؂�ւ���R���|�[�l���g�̒ǉ�
			/// </summary>
			/// <param name="param">�؂�ւ�������܂Ƃ߂��p�����[�^</param>
			void AddChangeComponent(const ChangeComponentParametor& param) {
				if (param.component == nullptr) {
					return;
				}
				m_changeParams.push_back(param);
			}

			/// <summary>
			/// �o�^���ꂽ�R���|�[�l���g�̐؂�ւ����s��(�X�^�[�g��)
			/// </summary>
			void StartChangeComponents() {
				for (auto& param : m_changeParams) {
					param.component->SetUpdateActive(param.isStart);
				}
			}

			/// <summary>
			/// �o�^���ꂽ�R���|�[�l���g�̐؂�ւ����s��(�I����)
			/// </summary>
			void ExitChangeComponents() {
				for (auto& param : m_changeParams) {
					param.component->SetUpdateActive(param.isExit);
				}
			}

		public:
			virtual void OnStart() override {
				StartChangeComponents();
			}

			virtual void OnExit() override {
				ExitChangeComponents();
			}

		};

		namespace StateNode {

			template<class OwnerType>
			class EmptyNode : public NodeBase_StateMachine<OwnerType>
			{
			public:
				EmptyNode(const int index, const std::shared_ptr<OwnerType>& owner) :
					NodeBase_StateMachine(index, owner)
				{}

				virtual ~EmptyNode() = default;

				void OnStart() override {}

				bool OnUpdate() override { return true; }

				void OnExit() override {}
			};

		}

	}
}