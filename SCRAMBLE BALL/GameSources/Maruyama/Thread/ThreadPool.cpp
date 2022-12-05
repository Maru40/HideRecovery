/*!
@file ThreadPool.cpp
@brief ThreadPoolクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	スレッドプール本体
	//--------------------------------------------------------------------------------------

	ThreadPool::ThreadPool(const std::uint_fast32_t& thread_count) :
		m_threadCount{ thread_count ? thread_count : std::thread::hardware_concurrency() }
	{
		m_threads.reset(new std::thread[m_threadCount]);

		for (std::uint_fast32_t i = 0; i < m_threadCount; ++i) {
			m_threads[i] = std::thread(&ThreadPool::Worker, this);
		}
	}

	ThreadPool::~ThreadPool() {
		{
			std::lock_guard<std::mutex> lock(m_tasksMutex);
			m_running = false;
		}

		m_condition.notify_all();

		for (std::uint_fast32_t i = 0; i < m_threadCount; i++) {
			m_threads[i].join();
		}
	}

	void ThreadPool::Worker() {
		for (;;) {
			std::function<void()> task;

			{
				//Task Queueが空でない、もしくは、ThreadPoolが終了するまで待機
				std::unique_lock<std::mutex> lock(m_tasksMutex);
				m_condition.wait(lock, [&] { return !m_tasks.empty() || !m_running; });

				//ThreadPoolが終了中で、TaskQueueが空の時は、ループを抜ける。 == スレッドが終了する。
				if (!m_running && m_tasks.empty()) {
					return;
				}

				//TaskQueueから、Taskを取り出す。
				task = std::move(m_tasks.front());
				m_tasks.pop();
			}

			task();
		}
	}


	//--------------------------------------------------------------------------------------
	///	テスタークラス
	//--------------------------------------------------------------------------------------

	namespace Tester {

		std::wstring say_ok(int number) {
			std::wstringstream debugTextStart;
			debugTextStart << L"[say_ok (" << std::this_thread::get_id << L")] >>> Start";
			Debug::GetInstance()->Log(debugTextStart.str());

			std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 何か重い処理
			std::wstringstream oss;
			oss << "[say_ok (" << std::this_thread::get_id() << ")] >>> Hello! number: " << number;

			std::wstringstream debugTextEnd;
			debugTextEnd << L"[say_ok (" << std::this_thread::get_id << L")] >>> End";
			Debug::GetInstance()->Log(debugTextEnd.str());

			return oss.str();
		}

		std::wstring TesterThread::say_hello(int number) {
			std::wstringstream debugTextStart;
			debugTextStart << L"[say_hello (" << std::this_thread::get_id << L")] >>> Start";
			Debug::GetInstance()->Log(debugTextStart.str());

			std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 何か重い処理
			std::wstringstream oss;
			oss << "[say_hello (" << std::this_thread::get_id() << ")] >>> Hello! number: " << number;

			std::wstringstream debugTextEnd;
			debugTextEnd << L"[say_hello (" << std::this_thread::get_id << L")] >>> End";
			Debug::GetInstance()->Log(debugTextEnd.str());

			return oss.str();
		}

		void TesterThread::Test() {
			ThreadPool executor;

			// スレッド数の確認
			int threadCount = executor.GetThreadCount();
			Debug::GetInstance()->Log(threadCount);

			// グローバル関数の非同期実行
			auto ok_future = executor.Submit(say_ok, 100);

			// メンバ関数の非同期実行
			auto hello_future = executor.Submit([&](auto number) { return say_hello(number); }, 999);

			// 結果の取得
			Debug::GetInstance()->Log(ok_future.get());
			Debug::GetInstance()->Log(hello_future.get());
		}

	}
}