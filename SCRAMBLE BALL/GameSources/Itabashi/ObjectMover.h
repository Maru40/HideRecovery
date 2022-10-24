#pragma once
#include "stdafx.h"

namespace basecross
{
	class VelocityManager;
	class UseWeapon;

namespace Operator
{
	/// <summary>
	/// 動く機能を持ったコンポーネント
	/// </summary>
	class ObjectMover : public Component
	{
		/// <summary>
		/// トランスフォーム
		/// </summary>
		std::weak_ptr<Transform> m_transform;
		/// <summary>
		/// カメラコンポーネント
		/// </summary>
		std::weak_ptr<Camera> m_camera;
		/// <summary>
		/// ベクトル管理コンポーネント
		/// </summary>
		std::weak_ptr<VelocityManager> m_velocityManager;
		/// <summary>
		/// 武器使用コンポーネント
		/// </summary>
		std::weak_ptr<UseWeapon> m_useWepon;

		/// <summary>
		/// カメラの影響を受けるか
		/// </summary>
		bool m_isCameraAffected = true;
		/// <summary>
		/// エイム中か
		/// </summary>
		bool m_isAim = false;
		/// <summary>
		/// 移動速度
		/// </summary>
		float m_moveSpeed = 1.0f;
		/// <summary>
		/// 移動に使用する前ベクトル
		/// </summary>
		Vec3 m_defaultForward = Vec3(0, 0, 1);

	public:
		ObjectMover(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;
		void OnLateStart() override;

		void OnUpdate() override {}

		void OnDraw() override {}

	private:
		/// <summary>
		/// 通常時の移動
		/// </summary>
		Vec3 DefaultMove(const Vec2& moveDirection);

		/// <summary>
		/// Aim中の移動
		/// </summary>
		Vec3 AimMove(const Vec2& moveDirection);

	public:

		/// <summary>
		/// 影響を受けるカメラを設定する
		/// </summary>
		/// <param name="camera">影響を与えるカメラ</param>
		void SetAffectedCamera(const std::shared_ptr<Camera>& camera) { m_camera = camera; }

		/// <summary>
		/// 影響を受けるカメラを取得する
		/// </summary>
		/// <returns>影響を与えるカメラ</returns>
		std::shared_ptr<Camera> GetAffectedCamera() const { return m_camera.lock(); }

		/// <summary>
		/// 移動速度を設定
		/// </summary>
		/// <param name="moveSpeed">移動速度</param>
		void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
		/// <summary>
		/// 移動速度を取得
		/// </summary>
		/// <returns>移動速度</returns>
		float GetMoveSpeed() const { return m_moveSpeed; }

		/// <summary>
		/// カメラの影響を受けるかを設定
		/// </summary>
		/// <param name="cameraAffected">カメラの影響を受けるか</param>
		void SetCameraAffected(bool cameraAffected) { m_isCameraAffected = cameraAffected; }
		/// <summary>
		/// カメラの影響を受けるかを取得
		/// </summary>
		/// <returns>カメラの影響を受けるか</returns>
		bool GetCameraAffected() const { return m_isCameraAffected; }

		/// <summary>
		/// ベクトル管理コンポーネントの設定
		/// </summary>
		/// <param name="velocityManager">ベクトル管理コンポーネント</param>
		void SetVelocityManager(const std::shared_ptr<VelocityManager>& velocityManager) { m_velocityManager = velocityManager; }
		/// <summary>
		/// ベクトル管理コンポーネントを取得
		/// </summary>
		/// <returns>ベクトル管理コンポーネント</returns>
		std::shared_ptr<VelocityManager> GetVelocityManager() const { return m_velocityManager.lock(); }

		/// <summary>
		/// 移動
		/// </summary>
		/// <param name="moveDirection">移動方向</param>
		/// <returns>移動量</returns>
		Vec3 Move(const Vec2& moveDirection);

		/// <summary>
		/// 移動
		/// </summary>
		/// <param name="x">xの移動方向</param>
		/// <param name="y">yの移動方向</param>
		void Move(float x, float y) { Move(Vec2(x, y)); }

		/// <summary>
		/// エイム中か
		/// </summary>
		/// <returns>エイム中ならtrue</returns>
		bool IsAim() const noexcept { return m_isAim; }

		/// <summary>
		/// エイム中かを設定
		/// </summary>
		/// <param name="isAim">エイム中か</param>
		void SetIsAim(const bool isAim) { m_isAim = isAim; }

		/// <summary>
		/// 移動に使用する前ベクトルの設定
		/// </summary>
		/// <param name="defaultForward">前ベクトル</param>
		void SetDefaultForward(const Vec3& defaultForward) { m_defaultForward = defaultForward; }

		/// <summary>
		/// 移動に使用する前ベクトルの取得
		/// </summary>
		/// <returns>前ベクトル</returns>
		const Vec3& GetDefaultForward() const { return m_defaultForward; }
	};
}
}