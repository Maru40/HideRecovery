/*!
@file I_Edge.h
@brief I_Edge�Ȃ�
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
			class I_Node;
			class I_PriorityController;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃G�b�W�C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Edge {
			public:
				virtual ~I_Edge() = default;

				/// <summary>
				/// ��Ԃ̃m�[�h��ݒ�
				/// </summary>
				/// <param name="node">��O�̃m�[�h</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��O�̃m�[�h���擾
				/// </summary>
				/// <returns>��O�̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// ��̃m�[�h�̐ݒ�
				/// </summary>
				/// <param name="node">��̃m�[�h</param>
				virtual void SetToNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��̃m�[�h���擾
				/// </summary>
				/// <returns>��̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetToNode() const = 0;

				/// <summary>
				/// �D��x�̐ݒ�
				/// </summary>
				/// <param name="priority">�D��x</param>
				virtual void SetPriority(const float priority) = 0;

				/// <summary>
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				virtual float GetPriority() const = 0;

				virtual void SetPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) = 0;

				virtual std::shared_ptr<I_PriorityController> GetPriorityContorller() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃G�b�W�̊��N���X
			//--------------------------------------------------------------------------------------
			class EdgeBase : public I_Edge
			{
				std::weak_ptr<I_Node> m_fromNode;	//�����̎�O�̃m�[�h
				std::weak_ptr<I_Node> m_toNode;		//�����̐�̃m�[�h
				std::shared_ptr<I_PriorityController> m_priorityContorller;	//�D��x�Ǘ�

			public:
				EdgeBase(
					const std::shared_ptr<I_Node>& fromNode, 
					const std::shared_ptr<I_Node>& toNode
				);

				EdgeBase(
					const std::shared_ptr<I_Node>& fromNode,
					const std::shared_ptr<I_Node>& toNode,
					const std::shared_ptr<I_PriorityController> priorityController
				);

				virtual ~EdgeBase() = default;

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void SetToNode(const std::shared_ptr<I_Node>& node) override { m_toNode = node; }

				std::shared_ptr<I_Node> GetToNode() const override { return m_toNode.lock(); }

				void SetPriority(const float priority) override;

				float GetPriority() const override;

				virtual void SetPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) override;

				virtual std::shared_ptr<I_PriorityController> GetPriorityContorller() override;
			};

		}
	}
}