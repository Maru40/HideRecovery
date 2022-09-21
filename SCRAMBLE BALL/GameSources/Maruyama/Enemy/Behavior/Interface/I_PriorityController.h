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
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				virtual float GetPriority() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̊��N���X
			//--------------------------------------------------------------------------------------
			class PriorityControllerBase : public I_PriorityController {
				float m_priority = 0;

			public:
				virtual ~PriorityControllerBase() = default;

				float GetPriority() const override { return m_priority; }
			};

		}
	}
}