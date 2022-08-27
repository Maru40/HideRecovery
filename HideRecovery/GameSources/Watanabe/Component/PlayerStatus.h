/*!
@file   PlayerStatus.h
@brief  プレイヤーステータスクラス
*/

#pragma once
#include "stdafx.h"
#include "I_Damaged.h"

#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	/// <summary>
	/// ステータス構造体
	/// </summary>
	struct Status {
		// HP
		int hp;
		// 最大HP（途中変更不可）
		const int maxHp;
		// スキルゲージ
		float skillGauge;

		Status(int maxHp)
			:hp(maxHp), maxHp(maxHp), skillGauge(0)
		{}

		/// <summary>
		/// ステータスのリセット
		/// </summary>
		void Reset() {
			hp = maxHp;
			skillGauge = 0;
		}
	};

	class PlayerStatus :public Component, public I_TeamMember {
	public:
		using DamageFuncType = std::function<void(const std::shared_ptr<PlayerStatus>&, const DamageData&)>;

	private:
		Status m_status;
		Team m_team;

		std::vector<DamageFuncType> m_damagedFuncs;
		std::weak_ptr<SoundEmitter> m_soundEmitter;

		SoundClip m_damageSoundClip;

	public:
		PlayerStatus(const shared_ptr<GameObject>& owner);

		void OnLateStart()override;
		void OnUpdate()override;
		void OnDraw()override {}

		/// <summary>
		/// ダメージを加える
		/// </summary>
		/// <param name="damage">ダメージデータ</param>
		void AddDamage(const DamageData& damage);

		/// <summary>
		/// 死んでいるか
		/// </summary>
		/// <returns>trueなら死んでいる</returns>
		bool IsDead();

		/// <summary>
		/// リスポーン（ステータスのリセットのみ）
		/// </summary>
		void Respawn();

		void AddFuncAddDamage(const DamageFuncType& damagedFunc);

		/// <summary>
		/// チームの設定
		/// </summary>
		/// <param name="team">設定するチーム</param>
		void SetTeam(const Team& team) override;

		/// <summary>
		/// チームの取得
		/// </summary>
		/// <returns>チーム</returns>
		Team GetTeam() const override { return m_team; }

		Status GetStatus() const { return m_status; }

		std::shared_ptr<GameObject> GetOwnerObject() const override { return GetGameObject(); }
	};
}
