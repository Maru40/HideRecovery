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
				/// �������Ɉ�x�����Ăԏ���
				/// </summary>
				virtual void OnCreate() = 0;

				/// <summary>
				/// �m�[�h�J�n���ɌĂяo�������B
				/// </summary>
				virtual void OnStart() = 0;

				/// <summary>
				/// �m�[�h�I�����ɌĂяo������
				/// </summary>
				virtual void OnExit() = 0;

				/// <summary>
				/// �J�ڏ����m�F�O�̏���(CanTransition���ĂԑO�ɌĂԏ���)
				/// </summary>
				virtual void ReserveCanTransition() = 0;

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

				virtual void OnCreate() override {}
				virtual void ReserveCanTransition() override {};
				virtual void OnStart() override {}
				virtual void OnExit() override {}

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