/*!
@file Ticket_AstarRoute.h
@brief Ticket_AstarRoute
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include <future>
#include <tuple>
#include <utility>

namespace basecross {

	class I_ThreadRequester;

	namespace Ticket
	{

		template<class R>
		class ThreadTicketBase
		{
		private:
			const I_ThreadRequester* m_requester;		//�`�P�b�g���s��
			std::future<R>	m_future;					//�X���b�h�ɂ��肢�����N���X�̖߂�l

			bool m_isValid;								//�L����Ԃ̐ݒ�

		public:
			ThreadTicketBase(
				const I_ThreadRequester* const requester,
				const std::future<R>& future
			) :
				m_requester(requester),
				m_future(future),
				m_isValid(false)
			{}

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �L����Ԃ̐ݒ�
			/// </summary>
			/// <param name="isValid">�L����Ԃł��邩�ǂ���</param>
			void SetValid(const bool isValid) { m_isValid = isValid; }

			/// <summary>
			/// �L����Ԃł��邩��Ԃ��B
			/// </summary>
			/// <returns>�L����ԂȂ�true</returns>
			bool IsValid() const { return m_isValid; }

			/// <summary>
			/// �t���[�`���[�f�[�^�̎擾
			/// </summary>
			/// <returns>�t���[�`���[�f�[�^</returns>
			_NODISCARD std::future<R> TakeFuture() { return std::move(m_future); }

		};

	}

}