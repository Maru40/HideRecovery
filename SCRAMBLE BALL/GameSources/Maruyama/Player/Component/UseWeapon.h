
/*!
@file UseWeapon.h
@brief UseWeaponなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class WeponBase;
	class RotationController;
	class VelocityManager;
	class PlayerAnimator;

	namespace maru {
		class ReactiveBool;
	}

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct UseWeapon_Parametor {
		float defaultCameraSpeed;
		float aimCameraSpeed;
		float assitPower;
		std::unique_ptr<maru::ReactiveBool> isAim;		//Aim中かどうか
		std::unique_ptr<maru::ReactiveBool> canShot;	//撃てる状態かどうか
		bool isUpdateRotation;	//向きの調整を更新するかどうか

		UseWeapon_Parametor();

		UseWeapon_Parametor(const bool isAim, const bool isShot, const bool isUpdateRotation = true);

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		UseWeapon_Parametor(const UseWeapon_Parametor& parametor);

		virtual ~UseWeapon_Parametor() = default;
	};

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラス
	//--------------------------------------------------------------------------------------
	class UseWeapon : public Component
	{
	public:
		using Parametor = UseWeapon_Parametor;

	private:
		Parametor m_param;					//パラメータ

		std::weak_ptr<WeponBase> m_wepon;	//武器

		std::weak_ptr<VelocityManager> m_velocityManager;		//速度管理
		std::weak_ptr<RotationController> m_rotationController;	//回転コントローラー
		std::weak_ptr<PlayerAnimator> m_animator;				//アニメーター

		std::vector<std::weak_ptr<GameObject>> m_players;

		std::weak_ptr<SoundEmitter> m_soundEmitter;

		Vec3 m_direction = Vec3(0, 0, 1);

		bool m_isUseCamera = false;

		SoundClip m_readyArmsSoundClip;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		UseWeapon(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="wepon">武器</param>
		UseWeapon(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<WeponBase>& wepon);

		virtual ~UseWeapon() = default;

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// エイム中の更新処理
		/// </summary>
		void AimUpdate();

		/// <summary>
		/// 回転更新
		/// </summary>
		void RotationUpdate();

		/// <summary>
		/// アシストで回転カメラを回転させる
		/// </summary>
		/// <param name="direction">向かせたい方向</param>
		void AssistCameraRotation(const Vec3& direction);

		/// <summary>
		/// アニメーション更新
		/// </summary>
		void AnimationUpdate();

		/// <summary>
		/// エイム状態を切り替えたときに呼び出したい処理の設定
		/// </summary>
		void SettingReactiveIsAim();

		/// <summary>
		/// 撃てる状態になったときに呼び出したい処理の登録
		/// </summary>
		void SettingReactiveCanShot();

		/// <summary>
		/// 回転する向きを計算して返す。
		/// </summary>
		/// <returns>回転する向き</returns>
		Vec3 CalculateRotationDirection();

		void SearchPlayers();

		void ChangeCameraSpeed(const float speed);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// エイム中の設定
		/// </summary>
		/// <param name="isAim">エイム中かどうか</param>
		void SetIsAim(const bool isAim);

		/// <summary>
		/// エイム中かどうか
		/// </summary>
		/// <returns>エイム中ならtrue</returns>
		bool IsAim() const;

		/// <summary>
		/// 撃てるかどうか
		/// </summary>
		/// <param name="isShot">撃てるならtrue</param>
		void SetCanShot(const bool canShot);

		/// <summary>
		/// 撃てるかどうか
		/// </summary>
		/// <returns>撃てるならtrue</returns>
		bool CanShot() const;

		/// <summary>
		/// 武器の設定
		/// </summary>
		/// <param name="wepon">武器</param>
		void SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept;

		/// <summary>
		/// 武器の取得
		/// </summary>
		/// <returns>武器</returns>
		std::shared_ptr<WeponBase> GetWepon() const noexcept;

		/// <summary>
		/// 武器の重さを取得
		/// </summary>
		/// <returns>武器の重さ</returns>
		float GetWeaponWeight() const noexcept;

		void SetDirection(const Vec3& direction) { m_direction = direction; }
		
		Vec3 GetDirection() const { return m_direction; }

		void SetIsUseCamera(bool isUseCamera) { m_isUseCamera = isUseCamera; }

		bool IsUseCamera() const { return m_isUseCamera; }

		void SetIsUpdateRotation(const bool isUpdateRotation) { m_param.isUpdateRotation = isUpdateRotation; }

		bool IsUpdateRoration() const { return m_param.isUpdateRotation; }

	};

}