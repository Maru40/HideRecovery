/*!
@file Ticket_AstarRoute.h
@brief Ticket_AstarRoute
担当者：丸山 裕喜
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
			const I_ThreadRequester* m_requester;		//チケット発行者
			std::future<R>	m_future;					//スレッドにお願いしたクラスの戻り値

			bool m_isValid;								//有効状態の設定

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
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 有効状態の設定
			/// </summary>
			/// <param name="isValid">有効状態であるかどうか</param>
			void SetValid(const bool isValid) { m_isValid = isValid; }

			/// <summary>
			/// 有効状態であるかを返す。
			/// </summary>
			/// <returns>有効状態ならtrue</returns>
			bool IsValid() const { return m_isValid; }

			/// <summary>
			/// フューチャーデータの取得
			/// </summary>
			/// <returns>フューチャーデータ</returns>
			_NODISCARD std::future<R> TakeFuture() { return std::move(m_future); }

		};

	}

}