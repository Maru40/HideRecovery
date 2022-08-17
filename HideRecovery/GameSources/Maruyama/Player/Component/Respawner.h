
/*!
@file Respawner.h
@brief Respawnerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class PlayerSpawnPoint;

	//--------------------------------------------------------------------------------------
	/// リスポーンをさせるクラス
	//--------------------------------------------------------------------------------------
	class Respawner : public Component
	{
		std::weak_ptr<PlayerSpawnPoint> m_spawnPoint;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		Respawner(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:

	public:
		/// <summary>
		/// リスポーン
		/// </summary>
		void Respawn();

		/// <summary>
		/// リスポーンできるかどうか
		/// </summary>
		/// <returns>リスポーンできるならtrue</returns>
		bool IsRespawn();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// スポーンポイントの設定
		/// </summary>
		/// <param name="spawnPoint">スポーンポイント</param>
		void SetSpawnPoint(const std::shared_ptr<PlayerSpawnPoint>& spawnPoint) { m_spawnPoint = spawnPoint; }

		/// <summary>
		/// スポーンポイントの取得
		/// </summary>
		/// <returns>スポーンポイント</returns>
		std::shared_ptr<PlayerSpawnPoint> GetSpawnPoint() const { return m_spawnPoint.lock(); }
	};

}