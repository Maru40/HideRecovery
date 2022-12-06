/*!
@file ThreadPool.cpp
@brief ThreadPoolクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ThreadPool.h"

#include "Patch/PlayerInputer.h"

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

		static std::mutex sm_mutex;

		std::wstring say_ok(int number, std::atomic<bool> isRuuning) {
			std::wstringstream debugTextStart;
			debugTextStart << L"[say_ok (" << std::this_thread::get_id << L")] >>> Start";
			sm_mutex.lock();
			Debug::GetInstance()->Log(debugTextStart.str());
			sm_mutex.unlock();

			//std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 何か重い処理
			//constexpr int loopCount = 1000000000;
			//for (int i = 0; i < loopCount; i++) {
			//	int s = 0;
			//}

			std::wstringstream oss;
			oss << "[say_ok (" << std::this_thread::get_id() << ")] >>> Hello! number: " << number;

			std::wstringstream debugTextEnd;
			debugTextEnd << L"[say_ok (" << std::this_thread::get_id << L")] >>> End";
			sm_mutex.lock();
			Debug::GetInstance()->Log(debugTextEnd.str());
			sm_mutex.unlock();

			return oss.str();
		}

		std::wstring TesterThreadObject::say_hello(int number, bool* isRunning) {
			std::wstringstream debugTextStart;
			debugTextStart << L"[say_hello (" << std::this_thread::get_id << L")] >>> Start";
			sm_mutex.lock();
			Debug::GetInstance()->Log(debugTextStart.str());
			sm_mutex.unlock();

			//std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 何か重い処理
			//constexpr int loopCount = static_cast<int>((float)INT_MAX * 1.0f);
			constexpr int loopCount = INT_MAX;
			int count = 0;
			for (int i = 0; i < loopCount; i++) {
				int s = 0;
			}

			std::wstringstream oss;
			oss << "[say_hello (" << std::this_thread::get_id() << ")] >>> Hello! number: " << number;

			std::wstringstream debugTextEnd;
			debugTextEnd << L"[say_hello (" << std::this_thread::get_id << L")] >>> End";
			sm_mutex.lock();
			Debug::GetInstance()->Log(debugTextEnd.str());
			//m_isRunning = false;
			sm_mutex.unlock();

			*isRunning = false;
			return oss.str();
		}

		TesterThreadObject::TesterThreadObject(const std::shared_ptr<Stage>& stage):
			GameObject(stage),
			m_futureData(new FutureData())
		{}

		void TesterThreadObject::Test() {
			ThreadPool& executor = m_executor;
			m_isRunning = true;

			// スレッド数の確認
			int threadCount = executor.GetThreadCount();
			Debug::GetInstance()->Log(threadCount);

			// グローバル関数の非同期実行
			std::atomic<bool> isisis = true;
			auto ok_future = executor.Submit(say_ok, 100, true);

			// メンバ関数の非同期実行
			m_futureData = std::make_shared<FutureData>();
			auto future = executor.Submit([&](int number, bool* isRunning) { return say_hello(number, isRunning); }, 999, &m_futureData->m_isRunning);
			m_futureData->MoveFuture(future);

			// 結果の取得
			Debug::GetInstance()->Log(ok_future.get());
		}

		void TesterThreadObject::OnUpdate() {
			if (PlayerInputer::GetInstance()->IsBDown()) {
				Test();
			}

			if (!m_futureData->m_isRunning && m_futureData->m_future.valid()) {
				Debug::GetInstance()->Log(m_futureData->m_future.get());
			}
		}

	}
}