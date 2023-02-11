
/*!
@file AIDirector_Ex.h
@brief AIDirector_Exなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

#include <future>
#include <tuple>
#include <utility>

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class ThreadPool;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class I_AIRequester;

		//--------------------------------------------------------------------------------------
		/// AIDirectorの改良版
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		private:
			std::unique_ptr<ThreadPool> m_threadPool;	//スレッドプール

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~AIDirector_Ex() = default;

			//リクエスト処理
			//C++14,17でそれぞれ別のテンプレートが推奨されているため、分岐
			#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
				/**
				* @brief Submit a function with zero or more arguments and a return value into the task queue,
				* and get a future for its eventual returned value.
				*/
				// C++17 から推奨されている, 関数の返り値の型R を取得する方法
					template <class F, class... Args,
						class R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
			#else
				// C++14 で, 関数の返り値の型R を取得する方法
					template <class F, class... Args,
						class R = typename std::result_of<std::decay_t<F>(std::decay_t<Args>...)>::type>
			#endif
			_NODISCARD std::future<R> Request(F&& function, Args... args) {
				return m_threadPool->Submit(function, args...);
			}
		};

	}
}