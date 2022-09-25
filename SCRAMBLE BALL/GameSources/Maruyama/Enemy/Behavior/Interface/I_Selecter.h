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
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			class I_PriorityController;

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
				/// ��ԗD��x�����m�[�h�̎擾
				/// </summary>
				/// <returns></returns>
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
				std::weak_ptr<I_Node> m_fromNode;									//�����̎�O�ɑ��݂���m�[�h
				std::vector<std::shared_ptr<TransitionNodeData>> m_transitionDatas;	//�����̑J�ڐ�m�[�h�Q(�D��x)

			public:
				virtual ~SelecterBase() = default;	//�f�X�g���N�^

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) override {
					m_transitionDatas.push_back(std::make_shared<TransitionNodeData>(priorityController, node));
				}

				/// <summary>
				/// �ŗD��̃m�[�h���擾
				/// </summary>
				/// <returns>�ŗD��m�[�h</returns>
				std::shared_ptr<I_Node> GetFirstPriorityNode() const override;

				bool IsEmptyTransitionNodes() const override;

			};

		}
	}
}