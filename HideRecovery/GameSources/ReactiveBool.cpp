
/*!
@file ReactiveBool.cpp
@brief ReactiveBoolのクラス実体
担当：丸山裕喜
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
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void ReactiveBool::AddFunction(const bool b, const std::function<void()>& function) {
			b ? m_trueAction.AddFunction(function) : m_falseAction.AddFunction(function);
		}

		bool ReactiveBool::GetValue() const {
			return this->value;
		}

		//--------------------------------------------------------------------------------------
		///	オペレータ
		//--------------------------------------------------------------------------------------

		bool ReactiveBool::operator =(const bool b) {
			if (this->value != b) { //違うのなら
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