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

				virtual void SetIsActive(const bool isActive) noexcept = 0;

				virtual bool IsActive() const noexcept = 0;

				/// <summary>
				/// �C���f�b�N�X�̃Z�b�g
				/// </summary>
				virtual void SetIndex(const int index) noexcept = 0;

				/// <summary>
				/// �C���f�b�N�X�̎擾
				/// </summary>
				/// <returns>�C���f�b�N�X</returns>
				virtual int GetIndex() const noexcept = 0;

				/// <summary>
				/// �^�C�v�̐ݒ�
				/// </summary>
				template<class EnumType>
				void SetType(const EnumType type) { SetIndex(static_cast<int>(type)); }

				/// <summary>
				/// �^�C�v�̎擾
				/// </summary>
				template<class EnumType>
				EnumType GetType() const { return static_cast<EnumType>(GetIndex()); }

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
				bool m_isActive = true;									//�A�N�e�B�u��Ԃ��ǂ���
				int m_index = 0;										//�m�[�h�C���f�b�N�X
				std::vector<std::shared_ptr<I_Decorator>> m_decorators;	//�f�R���[�^�z��

			public:
				virtual ~NodeBase() = default;

				virtual void SetIsActive(const bool isActive) noexcept override { m_isActive = isActive; };

				virtual bool IsActive() const noexcept override { return m_isActive; };

				void SetIndex(const int index) noexcept override { m_index = index; }

				int GetIndex() const noexcept override { return m_index; }

				bool CanTransition() const override;

				void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) override;

				bool IsDecoratorEmpty() const override;
			};

		}
	}
}