
/*!
@file MaruAction.h
@brief MaruAction�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�A�N�V�����N���X
		//--------------------------------------------------------------------------------------
		template<class T>
		class Action
		{
			std::vector<std::function<T>> m_functions; //�C�x���g�z��

		public:
			virtual ~Action() = default;

			/// <summary>
			/// �C�x���g��ǉ�����B
			/// </summary>
			/// <param name="function">�ǉ��������C�x���g</param>
			void AddFunction(const std::function<T>& function) {
				if (function) {
					m_functions.push_back(function);
				}
			}

			/// <summary>
			/// �o�^���ꂽ�C�x���g��S�ČĂяo���B
			/// </summary>
			/// <param name="params">�����p�����[�^�p�b�N</param>
			template<class... Ts>
			void Invoke(Ts&&... params) {
				for (auto& function : m_functions) {
					function(params...);
				}
			}

			/// <summary>
			/// �o�^�����C�x���g��S�ď���
			/// </summary>
			void Clear() {
				m_functions.clear();
			}

			//--------------------------------------------------------------------------------------
			///	�I�y���[�^
			//--------------------------------------------------------------------------------------

			Action operator +  (const std::function<T>& function) {
				m_functions.push_back(function);
				return *this;
			}

			Action operator += (const std::function<T>& function) {
				return *this + function;
			}

		};

	}
}