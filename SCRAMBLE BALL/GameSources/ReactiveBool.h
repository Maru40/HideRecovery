/*!
@file ReactiveBool.h
@brief ReactiveBool
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "MaruAction.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	���A�N�e�B�ubool
		//--------------------------------------------------------------------------------------
		class ReactiveBool
		{
		private:
			bool value;							//�{�̃f�[�^
			maru::Action<void()> m_trueAction;  //true�ɐ؂�ւ�������ɌĂяo���C�x���g
			maru::Action<void()> m_falseAction; //false�ɐ؂�ւ�������ɌĂяo���C�x���g

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ReactiveBool();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="b">bool�̏����l</param>
			ReactiveBool(const bool b);

			virtual ~ReactiveBool() = default;

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �؂�ւ����Ƃ��ɌĂяo���C�x���g��ǉ�
			/// </summary>
			/// <param name="b">�ǂ���ɐ؂�ւ����Ƃ��ɌĂяo����</param>
			/// <param name="function">�Ăяo���C�x���g</param>
			void AddFunction(const bool b, const std::function<void()>& function);

			/// <summary>
			/// �{�̃f�[�^�̎擾
			/// </summary>
			/// <returns>�{�̃f�[�^</returns>
			bool GetValue() const;

			//--------------------------------------------------------------------------------------
			///	�I�y���[�^
			//--------------------------------------------------------------------------------------

			bool operator = (const bool b);

			operator bool() const;

		};

	}
}

//endbasecross