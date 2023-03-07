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
	///	スレッドリクエスト者
	//--------------------------------------------------------------------------------------
	class I_ThreadRequester
	{
	public:
		virtual ~I_ThreadRequester() = default;
	};

	//--------------------------------------------------------------------------------------
	///	スレッド用タスクデータ
	//--------------------------------------------------------------------------------------
	struct ThreadTaskData
	{
		I_ThreadRequester* const requester;	//リクエスト者
		std::function<void()> task;			//タスク

		ThreadTaskData(
			I_ThreadRequester* const requester,
			const std::function<void()>& task
		);
	};

	//--------------------------------------------------------------------------------------
	///	スレッドプール
	//--------------------------------------------------------------------------------------
	class ThreadPool
	{
		std::unique_ptr<std::thread[]> m_threads;	//スレッド配列
		const std::uint_fast32_t m_threadCount;		//スレッド数

		//std::list<std::function<void()>> m_tasks;	//積まれたタスク
		std::list<std::shared_ptr<ThreadTaskData>> m_taskDatas;	//積まれたタスクデータ
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
		void PushTask(I_ThreadRequester* const requester, const F& task) {
			{
				const std::lock_guard<std::mutex> lock(m_tasksMutex);

				if (!m_running) {	//ThreadPoolが終了状態ならエラー
					throw std::runtime_error("Cannot schedule new task after shutdown");
				}

				//タスクの追加
				//m_tasks.push_back(std::function<void()>(task));
				m_taskDatas.push_back(std::make_shared<ThreadTaskData>(requester, std::function<void()>(task)));
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
		std::future<R> Submit(I_ThreadRequester* const requester, F&& func, Args... args) {
			auto task = std::make_shared<std::packaged_task<R(Args...)>>(func);	//タスクのシェアポインタを生成

			auto future = task->get_future();									//タスクのフューチャーデータを取得

			PushTask(requester, [task, args...]() { (*task)(args...); });		//タスクの登録

			return future;
		}

		void DeleteTask(I_ThreadRequester* const requester);

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

		class TesterThreadObject : public GameObject, public I_ThreadRequester {
		private:
			std::shared_ptr<ThreadPool> m_executor;	//メンバとして持たないと、スレッド終了時にjoinしてしまう。
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