
/*!
@file AIDirector_Ex.h
@brief AIDirector_Ex�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

#include <future>
#include <tuple>
#include <utility>

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class ThreadPool;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class I_AIRequester;

		//--------------------------------------------------------------------------------------
		/// AIDirector�̉��ǔ�
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		private:
			std::unique_ptr<ThreadPool> m_threadPool;	//�X���b�h�v�[��

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~AIDirector_Ex() = default;

			//���N�G�X�g����
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
			_NODISCARD std::future<R> Request(F&& function, Args... args) {
				return m_threadPool->Submit(function, args...);
			}
		};

	}
}