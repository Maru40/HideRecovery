
/*!
@file I_FactionMember.h
@brief I_FactionMemberなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		enum class FactionType;
		class EnemyBase;
		template<class T>
		class CoordinatorBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// グループメンバーのインターフェース
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
			FactionType m_factionType;										//グループタイプ
			std::weak_ptr<FactionCoordinator> m_factionCoordinator;			//グループ調整者
			std::weak_ptr<CoordinatorBase<EnemyBase>> m_assignedFaction;	//アサインしたグループ

		public:
			virtual ~I_FactionMember() = default;

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

			/// <summary>
			/// アサインされたグループの設定
			/// </summary>
			/// <param name="faction">アサインされたグループ</param>
			void SetAssignFaction(const std::shared_ptr<CoordinatorBase<EnemyBase>>& faction) { m_assignedFaction = faction; }

			/// <summary>
			/// アサインされたグループを取得
			/// </summary>
			/// <returns>アサインされたグループ</returns>
			std::shared_ptr<CoordinatorBase<EnemyBase>> GetAssignedFaction() const { return m_assignedFaction.lock(); }

			/// <summary>
			/// アサインされたグループをキャストして取得
			/// </summary>
			template<class T>
			std::shared_ptr<T> GetAssignedFaction() const { return dynamic_pointer_cast<T>(m_assignedFaction.lock()); }

		};

	}
}