
/*!
@file FactionCoordinator.h
@brief FactionCoordinatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class AIDirector;
		class EnemyBase;
		class CombatCoordinator;

		//--------------------------------------------------------------------------------------
		/// グループタイプ
		//--------------------------------------------------------------------------------------
		enum class FactionType
		{

		};

		//--------------------------------------------------------------------------------------
		/// グループメンバーのインターフェース
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
		public:

		};

		//--------------------------------------------------------------------------------------
		/// グループ調整者
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:

		private:
			std::weak_ptr<AIDirector> m_director;	//AIディレクター

				//パトロールグループ
			std::vector<std::shared_ptr<CombatCoordinator>> m_combats;	//攻撃用グループ
			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//全てのグループ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="director">AIディレクター</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			~FactionCoordinator() = default;

			void OnStart() override;
			void OnUpdate() override;
			void OnExit() override;

		private:
			

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ディレクターの取得
			/// </summary>
			/// <returns>ディレクター</returns>
			std::shared_ptr<AIDirector> GetDirector() const noexcept { return m_director.lock(); }

		};

	}
}