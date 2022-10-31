/*!
@file I_Decorator.h
@brief I_Decorator�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �f�R���[�^�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Decorator {
			public:
				virtual ~I_Decorator() = default;

				/// <summary>
				/// �J�ڂł��邩�ǂ���
				/// </summary>
				/// <returns>�J�ڂł���Ȃ�true</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// �A�b�v�f�[�g���\���ǂ���
				/// </summary>
				/// <returns>�A�b�v�f�[�g���\�Ȃ�true</returns>
				virtual bool CanUpdate() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �f�R���[�^�̊��N���X
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class DecoratorBase : public I_Decorator
			{
				std::weak_ptr<OwnerType> m_owner;	//�I�[�i�[

			public:
				DecoratorBase(const std::shared_ptr<OwnerType>& owner) :
					m_owner(owner)
				{}

				virtual ~DecoratorBase() = default;

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				_NODISCARD std::shared_ptr<OwnerType> GetOwner() const noexcept {
					return m_owner.lock();
				}
			};

		}
	}
}