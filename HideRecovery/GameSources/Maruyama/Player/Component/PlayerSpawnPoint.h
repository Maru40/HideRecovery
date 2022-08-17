
/*!
@file PlayerSpawnPoint.h
@brief PlayerSpawnPointなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// プレイヤーがスポーンされる位置管理クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct PlayerSpawnPoint_Parametor {
		Team team;	// チーム
		int id;		// 固有ID

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="team">チーム</param>
		/// <param name="id">固有ID</param>
		PlayerSpawnPoint_Parametor(const Team& team, const int id);
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーがスポーンされる位置管理クラス
	//--------------------------------------------------------------------------------------
	class PlayerSpawnPoint : public Component
	{
	public:
		using Parametor = PlayerSpawnPoint_Parametor;

	private:
		Parametor m_param;	//パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		PlayerSpawnPoint(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

	public:
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

		/// <summary>
		/// 固有IDの取得
		/// </summary>
		/// <returns>固有ID</returns>
		int GetID() const noexcept { return m_param.id; }

		Vec3 GetWorldPosition() const { return transform->GetWorldPosition(); }
	};

}