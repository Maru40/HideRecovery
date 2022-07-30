
/*!
@file FactionCoordinator.h
@brief FactionCoordinatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		class AIDirector;

		//グループ生成、メンバーのアサイン
		class FactionCoordinator
		{
		private:
			ex_weak_ptr<AIDirector> m_director;



		public:
			FactionCoordinator(const std::shared_ptr<AIDirector>& objPtr);

		};

	}
}