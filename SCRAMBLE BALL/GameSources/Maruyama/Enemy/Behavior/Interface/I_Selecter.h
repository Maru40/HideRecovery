/*!
@file I_Selecter.h
@brief I_Selecter�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

namespace basecross {
	template<class node_type, class EnumType, class TransitionMember>
	class EnemyMainStateMachine;

	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			class I_PriorityController;
			template<class EnumType>
			class BehaviorTree;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̃Z���N�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class SelectType {
				Priority,	//�D��x
				Sequence,	//�V�[�P���X
				Random,		//�����_��
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̑J�ڐ�m�[�h�f�[�^
			//--------------------------------------------------------------------------------------
			struct Selecter_TransitionNodeData {
				std::shared_ptr<I_PriorityController> priorityController;	//�D��x�R���g���[���[
				std::weak_ptr<I_Node> node;									//�m�[�h

				Selecter_TransitionNodeData(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				);

				virtual ~Selecter_TransitionNodeData() = default;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Selecter : public NodeBase {
			public:
				using TransitionNodeData = Selecter_TransitionNodeData;

			public:
				virtual ~I_Selecter() = default;

				virtual void OnStart() = 0;
				virtual bool OnUpdate() = 0;
				virtual void OnExit() = 0;

				/// <summary>
				/// �Z���N�g�^�C�v�̐ݒ�
				/// </summary>
				/// <param name="type">�Z���N�g�^�C�v</param>
				virtual void SetSelectType(const SelectType type) = 0;
				
				/// <summary>
				/// �Z���N�g�^�C�v�̎擾
				/// </summary>
				/// <returns>�Z���N�g�^�C�v</returns>
				virtual SelectType GetSelectType() const = 0;

				/// <summary>
				/// ��O�̃m�[�h�̐ݒ�
				/// </summary>
				/// <param name="node">��O�̃m�[�h</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��O�̃m�[�h�̎擾
				/// </summary>
				/// <returns>��O�̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// �J�ڐ�m�[�h�̒ǉ�
				/// </summary>
				/// <param name="priorityController">�D��x�Ǘ�</param>
				/// <param name="node">�J�ڐ�m�[�h</param>
				virtual void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) = 0;

				/// <summary>
				/// �ŗD��̃m�[�h���擾
				/// </summary>
				/// <returns>�ŗD��m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetFirstPriorityNode() const = 0;

				/// <summary>
				/// �J�ڐ�m�[�h���󂩂ǂ���
				/// </summary>
				/// <returns>�J�ڐ�m�[�h����Ȃ�true</returns>
				virtual bool IsEmptyTransitionNodes() const = 0;

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X
			//--------------------------------------------------------------------------------------

			class SelecterBase : public I_Selecter
			{
			public:
				//using StateMachine = EnemyMainStateMachine<>

			private:

				SelectType m_selectType;											//�Z���N�g�^�C�v
				std::weak_ptr<I_Node> m_fromNode;									//�����̎�O�ɑ��݂���m�[�h
				std::vector<std::shared_ptr<TransitionNodeData>> m_transitionDatas;	//�����̑J�ڐ�m�[�h�Q(�D��x)

				std::weak_ptr<TransitionNodeData> m_currentTransitionData;			//���ݎg�p���̃f�[�^

				//std::unique_ptr<>	//�X�e�[�^�[

			public:
				virtual ~SelecterBase() = default;	//�f�X�g���N�^

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

				void SetSelectType(const SelectType type) { m_selectType = type; }

				SelectType GetSelectType() const { return m_selectType; }

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) override {
					m_transitionDatas.push_back(std::make_shared<TransitionNodeData>(priorityController, node));
				}

				std::shared_ptr<I_Node> GetFirstPriorityNode() const override;

				bool IsEmptyTransitionNodes() const override;

				std::shared_ptr<I_Node> GetCurrentNode() const;

			private:

			};

		}
	}
}