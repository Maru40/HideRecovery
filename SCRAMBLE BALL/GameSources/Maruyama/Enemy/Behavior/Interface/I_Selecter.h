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
			class I_Edge;
			class I_PriorityController;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̃Z���N�g�^�C�v
			//--------------------------------------------------------------------------------------
			enum class SelectType {
				Priority,	//�D��x
				Sequence,	//�V�[�P���X
				Random,		//�����_��
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[
			//--------------------------------------------------------------------------------------
			class Selecter : public NodeBase
			{
			public:
				using SelecterType = SelectType;

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
				/// �V�[�P���T�Ƃ��Ă̑J�ڐ��I��
				/// </summary>
				/// <returns>�V�[�P���T�Ƃ��Ă̑J�ڐ�</returns>
				std::shared_ptr<I_Node> SearchSequenceNode() const;

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
				/// ���݂̃m�[�h��ݒ�
				/// </summary>
				/// <param name="node">�ݒ肷��m�[�h</param>
				void SetCurrentNode(const std::shared_ptr<I_Node>& node) noexcept;

				/// <summary>
				/// ���ݎg�p���̃m�[�h��Ԃ�
				/// </summary>
				_NODISCARD std::shared_ptr<I_Node> GetCurrentNode() const noexcept;

			};

		}
	}
}