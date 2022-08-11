
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirectorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// フィールド全体の制御
		//--------------------------------------------------------------------------------------
		class AIDirector : public maru::SingletonComponent<AIDirector>
		{
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//派閥の配列
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;

		private:
			/// <summary>
			/// エネミーを全て取得
			/// </summary>
			void SetEnemys();

			/// <summary>
			/// 派閥の追加
			/// </summary>
			/// <returns>生成した派閥</returns>
			std::shared_ptr<FactionCoordinator> AddFaction();

			/// <summary>
			/// 派閥の削除
			/// </summary>
			/// <param name="removeCoordinator">削除したい派閥</param>
			/// <returns>削除できたらtrue</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//アクセッサ-------------------------------------------------------------------------------------



		};

	}
}