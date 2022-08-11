
/*!
@file FactionCoordinator.h
@brief FactionCoordinatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class AIDirector;

		//--------------------------------------------------------------------------------------
		/// グループ生成、メンバーのアサイン
		//--------------------------------------------------------------------------------------
		class FactionCoordinator
		{
		private:
			std::weak_ptr<AIDirector> m_director;

		public:
			FactionCoordinator(const std::shared_ptr<AIDirector>& objPtr);

			void OnStart();
			void OnUpdate();
			void OnExit();

		private:
			

		public:
			/// <summary>
			/// ディレクターの取得
			/// </summary>
			/// <returns>ディレクター</returns>
			std::shared_ptr<AIDirector> GetDirector() const { return m_director.lock(); }

		};

	}
}