
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirectorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		class EnemyBase;
		class FactionCoordinator;

		//フィールド全体の制御
		class AIDirector : public maru::SingletonComponent<AIDirector>
		{
			vector<ex_weak_ptr<EnemyBase>> m_enemys;

			//FactionCoordinatorの管理
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;

		private:
			void SetEnemys();

			//Factionの追加、削除
			std::shared_ptr<FactionCoordinator> AddFaction();
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//アクセッサ-------------------------------------------------------------------------------------



		};

	}
}