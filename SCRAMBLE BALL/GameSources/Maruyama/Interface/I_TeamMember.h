/*!
@file I_TeamMember.h
@brief I_TeamMemberなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/Utility/Utility.h"

#include "Maruyama/Utility/ReactiveBool.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class OwnArea;

	namespace team {
		/// <summary>
		/// チームタイプ
		/// </summary>
		enum class TeamType {
			Blue,
			Red,
			Neutral
		};

		// 各チームの色（あいまいになることがあるのでbasecross::で明示する）
		const Col4 REDTEAM_COLOR = basecross::Utility::ConvertColorZeroToOne(Col4(243.0f, 17.0f, 67.0f, 255.0f));
		const Col4 BLUETEAM_COLOR = basecross::Utility::ConvertColorZeroToOne(Col4(0.0f, 150.0f, 226.0f, 255.0f));

		/// <summary>
		/// チームの色を取得
		/// </summary>
		/// <param name="teamType">チームタイプ</param>
		/// <returns>チームの色</returns>
		Col4 GetTeamColor(TeamType teamType);

		/// <summary>
		/// Enumを文字列に変換
		/// </summary>
		/// <param name="teamType">チームタイプ</param>
		/// <returns>チームタイプの文字列</returns>
		wstring GetTeamTypeString(TeamType teamType);
	}

	//--------------------------------------------------------------------------------------
	/// チームメンバーのインターフェース
	//--------------------------------------------------------------------------------------
	class I_TeamMember
	{
		maru::ReactiveBool m_isInArea = true;	//自陣にいるかどうか

		std::weak_ptr<OwnArea> m_ownArea;

	public:
		virtual ~I_TeamMember() = default;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 自陣エリアにいるかどうかの設定
		/// </summary>
		/// <param name="isInArea">自陣エリア内かどうか</param>
		void SetIsInArea(const bool isInArea) noexcept { m_isInArea = isInArea; }

		/// <summary>
		/// 自陣エリアにいるかどうか
		/// </summary>
		/// <returns>自陣エリアにいるならtrue</returns>
		bool IsInArea() const noexcept { return m_isInArea.GetValue(); }

		void AddReactiveIsInAreaEvent(const bool b, const std::function<void()>& func) {
			m_isInArea.AddFunction(b, func);
		}

		/// <summary>
		/// 自陣エリアの設定
		/// </summary>
		/// <param name="area">自陣エリア</param>
		void SetOwnArea(const std::shared_ptr<OwnArea>& area);

		/// <summary>
		/// 自陣エリアの取得
		/// </summary>
		/// <returns>自陣エリア</returns>
		std::shared_ptr<OwnArea> GetOwnArea() const;

		/// <summary>
		/// ゲームオブジェクトを取得
		/// </summary>
		/// <returns>ゲームオブジェクト</returns>
		virtual std::shared_ptr<GameObject> GetOwnerObject() const = 0;

		/// <summary>
		/// チームタイプの設定
		/// </summary>
		/// <param name="team">チームタイプ</param>
		virtual void SetTeam(const team::TeamType& team) = 0;

		/// <summary>
		/// チームタイプの取得
		/// </summary>
		/// <returns>チームタイプ</returns>
		virtual team::TeamType GetTeam() const = 0;
	};
}