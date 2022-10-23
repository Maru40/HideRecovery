
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
		//enum class FactionType;
		class EnemyBase;
		template<class T>
		class CoordinatorBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// グループメンバーのインターフェースのパラメータ
		//--------------------------------------------------------------------------------------
		struct I_FactionMember_Parametor
		{
			//FactionType factionType = FactionType(0);					//グループタイプ
			std::weak_ptr<FactionCoordinator> factionCoordinator;		//グループ調整者
			std::weak_ptr<CoordinatorBase<EnemyBase>> assignedFaction;	//アサインしたグループ
		};

		//--------------------------------------------------------------------------------------
		/// グループメンバーのインターフェース
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
		public:
			using Parametor = I_FactionMember_Parametor;

		private:
			Parametor m_param = Parametor();	//パラメータ

		public:
			virtual ~I_FactionMember() = default;

			///// <summary>
			///// グループタイプの設定
			///// </summary>
			///// <param name="type">グループタイプ</param>
			//void SetFactionType(const FactionType& type) noexcept { m_param.factionType = type; }

			///// <summary>
			///// グループタイプの取得
			///// </summary>
			///// <returns>グループタイプ</returns>
			//FactionType GetFactionType() const noexcept { m_param.factionType; }

			/// <summary>
			/// グループ調整者の設定
			/// </summary>
			/// <param name="faction">グループ調整者</param>
			void SetFactionCoordinator(const std::shared_ptr<FactionCoordinator>& faction) { m_param.factionCoordinator = faction; }

			/// <summary>
			/// グループ調整者の取得
			/// </summary>
			/// <returns>グループ調整者</returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator() const { return m_param.factionCoordinator.lock(); }

			/// <summary>
			/// アサインされたグループの設定
			/// </summary>
			/// <param name="faction">アサインされたグループ</param>
			void SetAssignFaction(const std::shared_ptr<CoordinatorBase<EnemyBase>>& faction) { m_param.assignedFaction = faction; }

			/// <summary>
			/// アサインされたグループを取得
			/// </summary>
			/// <returns>アサインされたグループ</returns>
			std::shared_ptr<CoordinatorBase<EnemyBase>> GetAssignedFaction() const { return m_param.assignedFaction.lock(); }

			/// <summary>
			/// アサインされたグループをキャストして取得
			/// </summary>
			/// <returns>キャストされたグループ</returns>
			template<class T>
			std::shared_ptr<T> GetAssignedFaction() const { return dynamic_pointer_cast<T>(m_param.assignedFaction.lock()); }

			/// <summary>
			/// ターゲットの設定
			/// </summary>
			/// <param name="target">ターゲット</param>
			virtual void SetTarget(const std::shared_ptr<GameObject>& target) = 0;

			/// <summary>
			/// ターゲットの取得
			/// </summary>
			/// <returns>ターゲット</returns>
			virtual std::shared_ptr<GameObject> GetTarget() const = 0;

			/// <summary>
			/// パラメータ設定
			/// </summary>
			/// <param name="param">パラメータ</param>
			void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

			/// <summary>
			/// パラメータの取得
			/// </summary>
			/// <returns>パラメータ</returns>
			Parametor GetParametor() const noexcept { return m_param; };

		};

	}
}