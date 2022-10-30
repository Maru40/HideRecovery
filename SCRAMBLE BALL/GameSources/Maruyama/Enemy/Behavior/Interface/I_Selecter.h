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

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			class I_Edge;
			class I_PriorityController;
			template<class EnumType>
			class BehaviorTree;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̃Z���N�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class SelectType {
				Priority,	//�D��x
				//Sequence,	//�V�[�P���X	//�V�[�P���T�[��ʂŗp�ӂ��邽�߁A�폜
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
			/// �r�w�C�r�A�Z���N�^�[�̃C���^�[�t�F�[�X(�g�p�֎~)
			//--------------------------------------------------------------------------------------
			class I_Selecter : public NodeBase {
			public:
				using TransitionNodeData = Selecter_TransitionNodeData;

			public:
				virtual ~I_Selecter() = default;

				//virtual void OnStart() = 0;
				//virtual bool OnUpdate() = 0;
				//virtual void OnExit() = 0;

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

				/// <summary>
				/// ���݃A�N�Z�X���̃m�[�h��؂�ւ���B
				/// </summary>
				/// <param name="node">�؂�ւ������m�[�h</param>
				virtual void ChangeCurrentNode(const std::shared_ptr<I_Node>& node) = 0;

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X(�g�p�֎~)
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
				SelecterBase();
				SelecterBase(const SelectType selectType);

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

				void ChangeCurrentNode(const std::shared_ptr<I_Node>& node) override;

			private:

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̉��ǌ^
			//--------------------------------------------------------------------------------------
			class Selecter : public NodeBase
			{
			private:
				std::weak_ptr<I_Node> m_currentNode;					//���ݎg�p���̃m�[�h

				SelectType m_selectType;								//�Z���N�g�^�C�v

				std::weak_ptr<I_Edge> m_fromEdge;						//��O�̃G�b�W
				std::vector<std::weak_ptr<I_Edge>> m_transitionEdges;	//�J�ڐ�̃G�b�W�z��

			public:
				Selecter();
				Selecter(const SelectType selectType);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:

				/// <summary>
				/// ��ԗD��x�̍����m�[�h��Ԃ��B
				/// </summary>
				/// <returns>��ԗD��x�������m�[�h</returns>
				std::shared_ptr<I_Node> SearchFirstPriorityNode() const;

				/// <summary>
				/// �����_���ɑJ�ڐ��I�ԁB
				/// </summary>
				/// <returns>�����_���ȑJ�ڐ�</returns>
				std::shared_ptr<I_Node> SearchRandomNode() const;

				/// <summary>
				/// ���ݎg�p���̃m�[�h��؂�ւ���B
				/// </summary>
				void ChangeCurrentNode(const std::shared_ptr<I_Node>& node);

			public:
				/// <summary>
				/// �X�V����m�[�h������
				/// </summary>
				std::shared_ptr<I_Node> SearchCurrentNode() const;

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				void SetSelectType(const SelectType type) { m_selectType = type; }

				SelectType GetSelectType() const { return m_selectType; }

				/// <summary>
				/// ��O�G�b�W�̐ݒ�
				/// </summary>
				void SetFromEdge(const std::shared_ptr<I_Edge>& fromEdge) noexcept;

				/// <summary>
				/// ��O�G�b�W�̎擾
				/// </summary>
				_NODISCARD std::shared_ptr<I_Edge> GetFromEdge() const noexcept;

				/// <summary>
				/// �J�ڐ�G�b�W�̒ǉ�
				/// </summary>
				void AddTransitionEdge(const std::shared_ptr<I_Edge>& edge);

				/// <summary>
				/// �J�ڐ�̃m�[�h�����݂��邩�ǂ���
				/// </summary>
				bool IsEmptyTransitionNodes() const;

				/// <summary>
				/// �J�����g�m�[�h�����݂��邩�ǂ���
				/// </summary>
				bool HasCurrentNode() const noexcept { return !m_currentNode.expired(); }

				/// <summary>
				/// ���ݎg�p���̃m�[�h��Ԃ�
				/// </summary>
				_NODISCARD std::shared_ptr<I_Node> GetCurrentNode() const noexcept;

			};

		}
	}
}