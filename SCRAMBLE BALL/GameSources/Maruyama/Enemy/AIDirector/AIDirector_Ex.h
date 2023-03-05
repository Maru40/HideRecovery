
/*!
@file AIDirector_Ex.h
@brief AIDirector_Exなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class ThreadPool;
	class AstarThreadController;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class I_AIRequester;
		class I_AIRequestEvent;
		struct AIRequestData;

		//--------------------------------------------------------------------------------------
		/// AIDirectorの改良版
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		public:

		private:
			std::unique_ptr<AstarThreadController> m_astarThread;	//AstarThreadController

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~AIDirector_Ex() = default;

		private:

		public:

			/// <summary>
			/// Astarスレッドの取得
			/// </summary>
			/// <returns></returns>
			_NODISCARD const std::unique_ptr<AstarThreadController>& GetAstarThreadController() const;

		};

	}
}