/*!
@file I_Node.h
@brief I_Node�Ȃ�
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
			class I_Decorator;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�m�[�h�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Node {
			public:
				virtual ~I_Node() = default;

				/// <summary>
				/// �C���f�b�N�X�̃Z�b�g
				/// </summary>
				virtual void SetIndex(const int index) noexcept = 0;

				/// <summary>
				/// �C���f�b�N�X�̎擾
				/// </summary>
				/// <returns></returns>
				virtual int GetIndex() const noexcept = 0;

				/// <summary>
				/// �J�ڂł��邩�ǂ���
				/// </summary>
				/// <returns>�J�ڂł���Ȃ�true</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// �f�R���[�^�̒ǉ�
				/// </summary>
				virtual void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) = 0;

				/// <summary>
				/// �f�R���[�^�����邩�ǂ���
				/// </summary>
				/// <returns>�f�R���[�^��Empty�Ȃ�true</returns>
				virtual bool IsDecoratorEmpty() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�m�[�h�̊��N���X
			//--------------------------------------------------------------------------------------
			class NodeBase : public I_Node
			{
				int m_index = 0;										//�m�[�h�C���f�b�N�X
				std::vector<std::shared_ptr<I_Decorator>> m_decorators;	//�f�R���[�^�z��

			public:
				virtual ~NodeBase() = default;

				void SetIndex(const int index) noexcept override { m_index = index; }

				int GetIndex() const noexcept override { return m_index; }

				bool CanTransition() const override;

				void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) override;

				bool IsDecoratorEmpty() const override;
			};

		}
	}
}