
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
		class PatrolCoordinator;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// グループタイプ
		//--------------------------------------------------------------------------------------
		enum class FactionType
		{
			None,	//所属していない
			Patrol,	//パトロール
			Combat,	//戦闘
		};

		//--------------------------------------------------------------------------------------
		/// グループメンバーのインターフェース
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
			FactionType m_factionType;								//グループタイプ
			std::weak_ptr<FactionCoordinator> m_factionCoordinator;	//グループ調整者

		public:
			/// <summary>
			/// グループタイプの設定
			/// </summary>
			/// <param name="type">グループタイプ</param>
			void SetFactionType(const FactionType& type) noexcept { m_factionType = type; }

			/// <summary>
			/// グループタイプの取得
			/// </summary>
			/// <returns>グループタイプ</returns>
			FactionType GetFactionType() const noexcept { m_factionType; }

			/// <summary>
			/// グループ調整者の設定
			/// </summary>
			/// <param name="faction">グループ調整者</param>
			void SetFactionCoordinator(const std::shared_ptr<FactionCoordinator>& faction) { m_factionCoordinator = faction; }

			/// <summary>
			/// グループ調整者の取得
			/// </summary>
			/// <returns>グループ調整者</returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator() const { return m_factionCoordinator.lock(); }

		};

		//--------------------------------------------------------------------------------------
		/// グループ調整者
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:

		private:
			std::weak_ptr<AIDirector> m_director;	//AIディレクター

			std::vector<std::shared_ptr<PatrolCoordinator>> m_patrols;			//パトロールグループ
			std::vector<std::shared_ptr<CombatCoordinator>> m_combats;			//攻撃用グループ
			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//全てのグループ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="director">AIディレクター</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="director">AIディレクター</param>
			/// <param name="members">メンバー配列</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members);

			~FactionCoordinator() = default;

			void OnStart() override;
			void OnUpdate() override;
			void OnExit() override;

		private:
			template<class T, class... Ts>
			std::shared_ptr<T> CreateFaction(std::vector<std::shared_ptr<T>>& addVec, Ts&&... params) {
				auto faction = std::make_shared<T>(params...);
				faction->OnStart();
				addVec.push_back(faction);
				m_allCoordinators.push_back(faction);

				return faction;
			}

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