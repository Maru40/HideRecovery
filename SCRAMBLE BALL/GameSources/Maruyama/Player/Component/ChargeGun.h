/*!
@file ChargeGun.h
@brief ChargeGunなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "WeponBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class ChargeBulletObject;
	class UseWeapon;
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	チャージ銃
	//--------------------------------------------------------------------------------------
	class ChargeGun : public WeponBase
	{
		std::weak_ptr<UseWeapon> m_useWepon;
		std::weak_ptr<SoundEmitter> m_soundEmitter;

		std::unique_ptr<GameTimer> m_timer;

		SoundClip m_shotSoundClip;

		team::TeamType m_teamType;
	public:
		ChargeGun(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

		std::shared_ptr<BulletObjectBase> Shot(const Vec3& direct) override;

	private:
		/// <summary>
		/// アニメーションの更新
		/// </summary>
		void UpdateAnimation();

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		void PlayAnimation();

		/// <summary>
		/// 撃つ方向の修正
		/// </summary>
		void RevisionShotDirection();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 撃てる状態かどうか
		/// </summary>
		/// <returns>撃てる状態ならtrue</returns>
		bool IsShot() const;
	};
}