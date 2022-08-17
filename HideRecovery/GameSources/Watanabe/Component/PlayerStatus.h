/*!
@file   PlayerStatus.h
@brief  プレイヤーステータスクラス
*/

#pragma once
#include "stdafx.h"
#include "I_Damaged.h"

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

	class PlayerStatus :public Component {
		Status m_status;
	public:
		PlayerStatus(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override {}
		void OnDraw()override {}

		/// <summary>
		/// ダメージを加える
		/// </summary>
		/// <param name="damage">ダメージデータ</param>
		void AddDamage(DamageData damage);
		/// <summary>
		/// 死んでいるか
		/// </summary>
		/// <returns>trueなら死んでいる</returns>
		bool IsDead();
		/// <summary>
		/// リスポーン（ステータスのリセットのみ）
		/// </summary>
		void Respawn();
	};
}
