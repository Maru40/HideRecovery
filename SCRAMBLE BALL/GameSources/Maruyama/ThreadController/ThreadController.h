/*!
@file ThreadController.h
@brief ThreadController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�X���b�h�R���g���[���[�̎���(���ݎg�p�֎~)
	//--------------------------------------------------------------------------------------
	template<class ResultType>
	class ThreadController 
	{
		bool m_isRunning;
		std::thread m_thread;

		ResultType result;	//���ʂ�ۑ�����B

	public:
		ThreadController(std::thread& newThread):
			m_isRunning(true)
		{
			newThread.swap(m_thread);

			std::thread observeRunning([&]() {
				m_thread.join();
				m_isRunning = false;
				Debug::GetInstance()->Log(L"�����I��");
			});

			observeRunning.detach();
		}

	};

}