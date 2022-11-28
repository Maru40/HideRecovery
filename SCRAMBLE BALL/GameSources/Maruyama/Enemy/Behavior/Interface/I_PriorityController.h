/*!
@file I_PriorityController.h
@brief I_PriorityController�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_PriorityController {
			public:
				virtual ~I_PriorityController() = default;

				/// <summary>
				/// �D��x�̌v�Z������
				/// </summary>
				/// <returns>�v�Z�����D��x</returns>
				virtual float CalculatePriority() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̊��N���X
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class PriorityControllerBase : public I_PriorityController {
				std::weak_ptr<OwnerType> m_owner;	//���L��

			public:
				PriorityControllerBase(const std::shared_ptr<OwnerType>& owner):
					m_owner(owner)
				{}

				virtual ~PriorityControllerBase() = default;

				/// <summary>
				/// ���L�҂̎擾
				/// </summary>
				/// <returns>���L��</returns>
				std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); }
			};

		}
	}
}