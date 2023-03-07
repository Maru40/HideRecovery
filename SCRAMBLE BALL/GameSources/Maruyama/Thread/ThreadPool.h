/*!
@file ThreadPool.h
@brief ThreadPool�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include <future>
#include <tuple>
#include <utility>

namespace basecross {

	//std::thread::hardware_concurrency() == �����n�ŃT�|�[�g�����X���b�h���s��

	//--------------------------------------------------------------------------------------
	///	�X���b�h���N�G�X�g��
	//--------------------------------------------------------------------------------------
	class I_ThreadRequester
	{
	public:
		virtual ~I_ThreadRequester() = default;
	};

	//--------------------------------------------------------------------------------------
	///	�X���b�h�p�^�X�N�f�[�^
	//--------------------------------------------------------------------------------------
	struct ThreadTaskData
	{
		I_ThreadRequester* const requester;	//���N�G�X�g��
		std::function<void()> task;			//�^�X�N

		ThreadTaskData(
			I_ThreadRequester* const requester,
			const std::function<void()>& task
		);
	};

	//--------------------------------------------------------------------------------------
	///	�X���b�h�v�[��
	//--------------------------------------------------------------------------------------
	class ThreadPool
	{
		std::unique_ptr<std::thread[]> m_threads;	//�X���b�h�z��
		const std::uint_fast32_t m_threadCount;		//�X���b�h��

		std::list<std::shared_ptr<ThreadTaskData>> m_taskDatas;	//�ς܂ꂽ�^�X�N�f�[�^
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
		void PushTask(I_ThreadRequester* const requester, const F& task) {
			{
				const std::lock_guard<std::mutex> lock(m_tasksMutex);

				if (!m_running) {	//ThreadPool���I����ԂȂ�G���[
					throw std::runtime_error("Cannot schedule new task after shutdown");
				}

				//�^�X�N�̒ǉ�
				//m_tasks.push_back(std::function<void()>(task));
				m_taskDatas.push_back(std::make_shared<ThreadTaskData>(requester, std::function<void()>(task)));
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
			class R = typename std::result_of<std::decay_t<F>(std::decay_t<Args>...)>::type>
#endif
		std::future<R> Submit(I_ThreadRequester* const requester, F&& func, Args... args) {
			auto task = std::make_shared<std::packaged_task<R(Args...)>>(func);	//�^�X�N�̃V�F�A�|�C���^�𐶐�

			auto future = task->get_future();									//�^�X�N�̃t���[�`���[�f�[�^���擾

			PushTask(requester, [task, args...]() { (*task)(args...); });		//�^�X�N�̓o�^

			return future;
		}

		/// <summary>
		/// �^�X�N�̍폜
		/// </summary>
		/// <param name="requester">�\����</param>
		void DeleteTask(I_ThreadRequester* const requester);

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
			std::shared_ptr<ThreadPool> m_executor;	//�����o�Ƃ��Ď����Ȃ��ƁA�X���b�h�I������join���Ă��܂��B
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