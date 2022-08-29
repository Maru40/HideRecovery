/*!
@file OwnArea.h
@brief OwnAreaなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "ImpactMap.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class I_TeamMember;
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// 自陣エリアのパラメータ
	//--------------------------------------------------------------------------------------
	struct OwnArea_Parametor {
		team::TeamType team;		//チーム
		float radius;	//エリア範囲の半径
		maru::Rect rect;	//エリア範囲

		OwnArea_Parametor(const team::TeamType& team, const float radius);
	};

	//--------------------------------------------------------------------------------------
	/// 自陣エリア
	//--------------------------------------------------------------------------------------
	class OwnArea : public Component
	{
	public:
		using Parametor = OwnArea_Parametor;

	private:
		Parametor m_param;	//パラメータ

		std::vector<std::weak_ptr<I_TeamMember>> m_members;	//エリアに所属しているメンバー一覧

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		OwnArea(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnLateStart() override;

		void OnUpdate() override;

	private:
		void SearchPlayers();

	public:
		void OnCollisionEnter(const CollisionPair& pair) override;

		void OnCollisionExit(const CollisionPair& pair) override;

		/// <summary>
		/// 自陣エリア内にいるかどうか判断
		/// </summary>
		/// <param name="member">確認を取りたいメンバー</param>
		/// <returns>自陣エリア内にいるならtrue</returns>
		bool IsInArea(const std::shared_ptr<I_TeamMember>& member);

		/// <summary>
		/// 自陣エリア内にいるかどうか
		/// </summary>
		/// <param name="object">確認を取りたいメンバー</param>
		/// <returns>自陣エリア内にいるならtrue</returns>
		bool IsInArea(const std::shared_ptr<GameObject>& member);

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// チームの設定
		/// </summary>
		/// <param name="team">チーム</param>
		void SetTeam(const team::TeamType& team) noexcept { m_param.team = team; }

		/// <summary>
		/// チームの取得
		/// </summary>
		/// <returns>チーム</returns>
		team::TeamType GetTeam() const noexcept { return m_param.team; }

		/// <summary>
		/// エリア半径の設定
		/// </summary>
		/// <param name="radius">エリア半径</param>
		void SetRadius(const float radius) noexcept { m_param.radius = radius; }

		/// <summary>
		/// エリア半径の取得
		/// </summary>
		/// <returns>エリア半径</returns>
		float GetRadius() const noexcept { return m_param.radius; }

		/// <summary>
		/// メンバーの追加
		/// </summary>
		/// <param name="member">メンバー</param>
		void AddMember(const std::shared_ptr<I_TeamMember>& member);

		/// <summary>
		/// メンバーの削除
		/// </summary>
		/// <param name="member">削除するメンバー</param>
		bool RemoveMember(const std::shared_ptr<I_TeamMember>& member);
	};
}