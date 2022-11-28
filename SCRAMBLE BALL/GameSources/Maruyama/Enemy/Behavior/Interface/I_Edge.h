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
				virtual void SetPriority(const float priority) noexcept = 0;

				/// <summary>
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				virtual float GetPriority() const noexcept = 0;

				/// <summary>
				/// �D��x�̌v�Z
				/// </summary>
				/// <returns>�v�Z��̗D��x</returns>
				virtual float CalculatePriority() = 0;

				/// <summary>
				/// �D��x�Ǘ��̒ǉ�
				/// </summary>
				/// <param name="priorityController">�D��x�Ǘ��N���X</param>
				virtual void AddPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃G�b�W�̊��N���X
			//--------------------------------------------------------------------------------------
			class EdgeBase : public I_Edge
			{
			public:
				using PriorityControllers = std::vector<std::shared_ptr<I_PriorityController>>;	//�D��x�Ǘ��Q

			private:
				std::weak_ptr<I_Node> m_fromNode;	//�����̎�O�̃m�[�h
				std::weak_ptr<I_Node> m_toNode;		//�����̐�̃m�[�h

				float m_priority;					//�D��x
				
				PriorityControllers m_priorityControllers;	//�D��x�Ǘ��Q

			public:
				EdgeBase(
					const std::shared_ptr<I_Node>& fromNode, 
					const std::shared_ptr<I_Node>& toNode,
					const float priority
				);

				virtual ~EdgeBase() = default;

				/// <summary>
				/// ��Ԃ̃m�[�h��ݒ�
				/// </summary>
				/// <param name="node">��O�̃m�[�h</param>
				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				/// <summary>
				/// ��O�̃m�[�h���擾
				/// </summary>
				/// <returns>��O�̃m�[�h</returns>
				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				/// <summary>
				/// ��̃m�[�h�̐ݒ�
				/// </summary>
				/// <param name="node">��̃m�[�h</param>
				void SetToNode(const std::shared_ptr<I_Node>& node) override { m_toNode = node; }

				/// <summary>
				/// ��̃m�[�h���擾
				/// </summary>
				/// <returns>��̃m�[�h</returns>
				std::shared_ptr<I_Node> GetToNode() const override { return m_toNode.lock(); }

				/// <summary>
				/// �D��x�̐ݒ�
				/// </summary>
				/// <param name="priority">�D��x</param>
				void SetPriority(const float priority) noexcept override { m_priority = priority; };

				/// <summary>
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				float GetPriority() const noexcept override { return m_priority; }

				/// <summary>
				/// �D��x�̌v�Z
				/// </summary>
				/// <returns>�v�Z��̗D��x</returns>
				float CalculatePriority() override;

				/// <summary>
				/// �D��x�Ǘ��̒ǉ�
				/// </summary>
				/// <param name="priorityController">�D��x�Ǘ��N���X</param>
				void AddPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) override;

				/// <summary>
				/// �D��x�Ǘ���ǉ�
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_base_of_v<I_PriorityController, T> &&	//���N���X
						std::is_constructible_v<T, Ts...>,				//�R���X�g���N�^
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> AddPriorityController(Ts&&... params) {
					auto newController = std::make_shared<T>(params...);
					AddPriorityController(newController);

					return newController;
				}
			};

		}
	}
}