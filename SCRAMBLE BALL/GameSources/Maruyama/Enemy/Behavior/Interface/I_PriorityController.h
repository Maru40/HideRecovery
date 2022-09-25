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
				/// �D��x�̐ݒ�
				/// </summary>
				/// <param name="priority">�D��x</param>
				virtual void SetPriority(const float priority) = 0;

				/// <summary>
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				virtual float GetPriority() const = 0;

				/// <summary>
				/// �D��x�̌v�Z������
				/// </summary>
				/// <returns>�v�Z�����D��x</returns>
				virtual float CalculatePriority() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̊��N���X
			//--------------------------------------------------------------------------------------
			class PriorityControllerBase : public I_PriorityController {
				float m_priority = 0;

			public:
				PriorityControllerBase();
				PriorityControllerBase(const float priority);

				virtual ~PriorityControllerBase() = default;

				void SetPriority(const float priority) override { m_priority = priority; };

				float GetPriority() const override { return m_priority; }

				float CalculatePriority() override { return m_priority; }	//�{���͏������z�֐��ɂ���\��
			};

		}
	}
}