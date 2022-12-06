/*!
@file ThreadPool.h
@brief ThreadPool�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include <future>

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//std::thread::hardware_concurrency() == �����n�ŃT�|�[�g�����X���b�h���s��

	//--------------------------------------------------------------------------------------
	///	�X���b�h�v�[��
	//--------------------------------------------------------------------------------------
	class ThreadPool
	{
		std::unique_ptr<std::thread[]> m_threads;	//�X���b�h�z��
		const std::uint_fast32_t m_threadCount;		//�X���b�h��

		std::queue<std::function<void()>> m_tasks;	//�ς܂ꂽ�^�X�N
		mutable std::mutex m_tasksMutex{};			//�~���[�e�b�N�X	

		std::atomic<bool> m_running = true;			//�^�X�N��Active

		//TaskQueue�ւ̒ǉ����������ꍇ�ȂǂɊO������notify()�����܂őҋ@����B
		//�N�����ATaskQueue����łȂ��A�������́AThreadPool���I����ԂłȂ��ꍇ�A�Ăёҋ@����B
		std::condition_variable m_condition;		//�ҋ@�����n

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="thread_count">�쐬�������X���b�h��</param>
		ThreadPool(const std::uint_fast32_t& thread_count = std::thread::hardware_concurrency());

		virtual ~ThreadPool();

	private:
		/// <summary>
		/// �X���b�h�̌Ăяo���B
		/// </summary>
		void Worker();

		/// <summary>
		/// �^�X�N�̒ǉ�
		/// </summary>
		template<class F>
		void PushTask(const F& task) {
			{
				const std::lock_guard<std::mutex> lock(m_tasksMutex);

				if (!m_running) {	//ThreadPool���I����ԂȂ�G���[
					throw std::runtime_error("Cannot schedule new task after shutdown");
				}

				//�^�X�N�̒ǉ�
				m_tasks.push(std::function<void()>(task));
			}

			//�ҋ@�X���b�h�̋N��
			m_condition.notify_one();
		}

	public:

//�X���b�h�ǉ�����
//C++14,17�ł��ꂼ��ʂ̃e���v���[�g����������Ă��邽�߁A����
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
		/**
		* @brief Submit a function with zero or more arguments and a return value into the task queue,
		* and get a future for its eventual returned value.
		*/
		// C++17 ���琄������Ă���, �֐��̕Ԃ�l�̌^R ���擾������@
		template <class F, class... Args, 
			class R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
#else
		// C++14 ��, �֐��̕Ԃ�l�̌^R ���擾������@
		template <class F, class... Args, 
			class R = class std::result_of<std::decay_t<F>(std::decay_t<Args>...)>::type>
#endif
		std::future<R> Submit(F&& func, const Args&&... args) {
			auto task = std::make_shared<std::packaged_task<R()>>([func, args...]() {
				return func(args...);
			});

			auto future = task->get_future();

			PushTask([task]() { (*task)(); });
			return future;
		}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X���b�h�J�E���g�̎擾
		/// </summary>
		/// <returns>�X���b�h�J�E���g</returns>
		std::uint_fast32_t GetThreadCount() const noexcept { return m_threadCount; }

	};

	

	//--------------------------------------------------------------------------------------
	///	�e�X�^�[�N���X
	//--------------------------------------------------------------------------------------

	namespace Tester {

		class TesterThread {
		private:
			std::wstring say_hello(int number);

			bool m_isRuning = true;

		public:
			void Test();
		};

	}



}