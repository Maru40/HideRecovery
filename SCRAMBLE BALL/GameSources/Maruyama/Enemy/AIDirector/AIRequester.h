
/*!
@file I_AIRequster.h
@brief I_AIRequsterなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// AIのリクエストする者
		//--------------------------------------------------------------------------------------
		class I_AIRequester
		{
		public:
			virtual ~I_AIRequester() = default;
		};

		//--------------------------------------------------------------------------------------
		/// AIリクエストエベントのインターフェース
		//--------------------------------------------------------------------------------------
		class I_AIRequestEvent
		{
		public:
			virtual ~I_AIRequestEvent() = default;
		};

		//--------------------------------------------------------------------------------------
		/// リクエストデータ
		//--------------------------------------------------------------------------------------
		struct AIRequestData
		{
			std::weak_ptr<I_AIRequester> requester;	//リクエスト者
			I_AIRequestEvent* eventPtr;				//処理したいイベント

			AIRequestData(const std::shared_ptr<I_AIRequester>& requester, I_AIRequestEvent* const eventPtr);

			virtual ~AIRequestData() = default;

		public:
			void NormalTest() {}

			void ConstTest() const {}
		};

	}
}