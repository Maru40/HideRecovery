/*!
@file ThreadPool.h
@brief ThreadPoolなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include <future>
#include <tuple>
#include <utility>

namespace basecross {

	//std::thread::hardware_concurrency() == 処理系でサポートされるスレッド並行数

	//--------------------------------------------------------------------------------------
	///	スレッドプール
	//--------------------------------------------------------------------------------------
	class ThreadPool
	{
		std::unique_ptr<std::thread[]> m_threads;	//スレッド配列
		const std::uint_fast32_t m_threadCount;		//スレッド数

		std::queue<std::function<void()>> m_tasks;	//積まれたタスク
		mutable std::mutex m_tasksMutex{};			//ミューテックス	

		std::atomic<bool> m_running = true;			//タスクのActive

		//TaskQueueへの追加があった場合などに外部からnotify()されるまで待機する。
		//起床時、TaskQueueが空でない、もしくは、ThreadPoolが終了状態でない場合、再び待機する。
		std::condition_variable m_condition;		//待機処理系

	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="thread_count">作成したいスレッド数</param>
		ThreadPool(const std::uint_fast32_t& thread_count = std::thread::hardware_concurrency());

		virtual ~ThreadPool();

	private:
		/// <summary>
		/// スレッドの呼び出し。
		/// </summary>
		void Worker();

		/// <summary>
		/// タスクの追加
		/// </summary>
		template<class F>
		void PushTask(const F& task) {
			{
				const std::lock_guard<std::mutex> lock(m_tasksMutex);

				if (!m_running) {	//ThreadPoolが終了状態ならエラー
					throw std::runtime_error("Cannot schedule new task after shutdown");
				}

				//タスクの追加
				m_tasks.push(std::function<void()>(task));
			}

			//待機スレッドの起床
			m_condition.notify_one();
		}

	public:

//スレッド追加処理
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
		std::future<R> Submit(F&& func, Args... args) {
			//auto task = std::make_shared<std::packaged_task<R()>>([func, args...]() {
			//	return func(args...);
			//});

			auto task = std::make_shared<std::packaged_task<R(Args...)>>(func);

			auto future = task->get_future();
	
			//PushTask([task]() { (*task)(); });
			//PushTask([task, args...]() { (*task)(std::forward<Args>(args)...); });

			//(*task)(std::move(args)...);

			//[ar = std::move(args)](){};

			//[task, tup = std::make_tuple(std::move(args)...)]{
			//	
			//};

			//(*task)(std::forward<Args>(args)...);
			//decltype(args) ar = std::forward<Args>(args);
			//[ar = std::forward_as_tuple(args)]() {};

			//decltype(args...) ar = std::move(args)...;

			//[args = std::move(args)...]() -> decltype(auto) {};

			PushTask([task, args...]() { (*task)(args...); });

			return future;
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// スレッドカウントの取得
		/// </summary>
		/// <returns>スレッドカウント</returns>
		std::uint_fast32_t GetThreadCount() const noexcept { return m_threadCount; }

	};

	

	//--------------------------------------------------------------------------------------
	///	テスタークラス
	//--------------------------------------------------------------------------------------

	namespace Tester {

		struct FutureData {
			static int count;

			bool m_isRunning;
			std::future<std::wstring> m_future;

			FutureData() :
				//FutureData(std::future<std::wstring>())
				m_isRunning(true)
			{
				count++;
			}

			FutureData(std::future<std::wstring>& newFuture) :
				m_isRunning(true),
				m_future(std::move(newFuture))
			{
				count++;
			}

			~FutureData() {
				count--;
			}

			void MoveFuture(std::future<std::wstring>& other) {
				m_future = std::move(other);
			}
		};

		class TesterThreadObject : public GameObject {
		private:
			ThreadPool m_executor;	//メンバとして持たないと、スレッド終了時にjoinしてしまう。
			//std::future<std::wstring> m_hello_future;

			std::atomic<bool> m_isRunning = false;

			std::shared_ptr<FutureData> m_futureData;

			std::wstring say_hello(std::shared_ptr<FutureData>& data);

		public:
			TesterThreadObject(const std::shared_ptr<Stage>& stage);

			void Test();

			void OnUpdate() override;
		};

	}



}