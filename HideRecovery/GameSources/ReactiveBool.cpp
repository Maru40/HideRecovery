
/*!
@file ReactiveBool.cpp
@brief ReactiveBool�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ReactiveBool.h"

namespace basecross {

	namespace maru {

		ReactiveBool::ReactiveBool()
			:ReactiveBool(false)
		{}

		ReactiveBool::ReactiveBool(const bool b)
			:value(b)
		{}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void ReactiveBool::AddFunction(const bool b, const std::function<void()>& function) {
			b ? m_trueAction.AddFunction(function) : m_falseAction.AddFunction(function);
		}

		bool ReactiveBool::GetValue() const {
			return this->value;
		}

		//--------------------------------------------------------------------------------------
		///	�I�y���[�^
		//--------------------------------------------------------------------------------------

		bool ReactiveBool::operator =(const bool b) {
			if (this->value != b) { //�Ⴄ�̂Ȃ�
				b ? m_trueAction.Invoke() : m_falseAction.Invoke();
			}

			this->value = b;
			return this->value;
		}

		ReactiveBool::operator bool() const {
			return this->value;
		}

	}
}