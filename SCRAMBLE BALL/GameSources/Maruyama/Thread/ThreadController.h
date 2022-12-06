/*!
@file ThreadController.h
@brief ThreadControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	スレッドコントローラーの実装(現在使用禁止)
	//--------------------------------------------------------------------------------------
	template<class ResultType>
	class ThreadController 
	{
		bool m_isRunning;
		std::thread m_thread;

		ResultType result;	//結果を保存する。

	public:
		ThreadController(std::thread& newThread):
			m_isRunning(true)
		{
			newThread.swap(m_thread);

			std::thread observeRunning([&]() {
				m_thread.join();
				m_isRunning = false;
				Debug::GetInstance()->Log(L"検索終了");
			});

			observeRunning.detach();
		}

	};

}