
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
			//全てのエネミー
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//グループ管理の配列
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// 初期配置されたエネミーを全て取得
			/// </summary>
			void SettingStartAllEnemys();

			/// <summary>
			/// グループ管理の生成
			/// </summary>
			/// <returns>生成した派閥</returns>
			std::shared_ptr<FactionCoordinator> CreateFaction();

			/// <summary>
			/// グループ管理の削除
			/// </summary>
			/// <param name="removeCoordinator">削除したい派閥</param>
			/// <returns>削除できたらtrue</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// エネミーの追加
			/// </summary>
			/// <param name="enemy">エネミー</param>
			void AddEnemy(const std::shared_ptr<EnemyBase>& enemy);

			/// <summary>
			/// グループ管理の取得
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator(const int index) const;

		};

	}
}