/*!
@file Goar.h
@brief Goarクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct Goar_Parametor {
		Team team;

		Goar_Parametor(const Team& team);
	};

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス
	//--------------------------------------------------------------------------------------
	class Goar : public Component
	{
	public:
		using Parametor = Goar_Parametor;

	private:
		Parametor m_param;

	public:
		Goar(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ゴール成功
		/// </summary>
		/// <param name="pair">ゴールをした人</param>
		void SuccessGoar(const CollisionPair& pair);

		/// <summary>
		/// 当たり判定をとるかどうか
		/// </summary>
		/// <param name="pair">当たった相手のデータ</param>
		/// <returns>当たり判定をとるならtrue</returns>
		bool IsCollision(const CollisionPair& pair) const;

	public:
		void OnCollisionEnter(const CollisionPair& pair) override;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// チームタイプの設定
		/// </summary>
		/// <param name="team">チームタイプ</param>
		void SetTeam(const Team& team) noexcept { m_param.team = team; }

		/// <summary>
		/// チームタイプの取得
		/// </summary>
		/// <returns>チームタイプ</returns>
		Team GetTeam() const noexcept { return m_param.team; }

	};

}