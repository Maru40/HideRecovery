
/*!
@file AIDirector_Ex.h
@brief AIDirector_Exなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class I_AIRequester;

		//--------------------------------------------------------------------------------------
		/// AIDirectorの改良版
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		private:
			

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			//命令をお願いする。
			void Request(std::shared_ptr<I_AIRequester>& requester);
		};

	}
}