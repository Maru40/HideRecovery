#pragma once
#include "stdafx.h"

namespace basecross
{
	class VelocityManager;

namespace Operator
{
	class ObjectMover : public Component
	{
		std::weak_ptr<Transform> m_transform;
		std::weak_ptr<Camera> m_camera;
		std::weak_ptr<VelocityManager> m_velocityManager;

		bool m_isCameraAffected = true;
		bool m_isAim = false;

		float m_moveSpeed = 1.0f;

		Vec3 m_defaultForward = Vec3(0, 0, 1);

	public:
		ObjectMover(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override {}

		void OnDraw() override {}

	private:
		/// <summary>
		/// NormalMove
		/// </summary>
		Vec3 NormalMove(const Vec2& moveDirection);

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

		void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
		float GetMoveSpeed() const { return m_moveSpeed; }

		void SetCameraAffected(bool cameraAffected) { m_isCameraAffected = cameraAffected; }
		bool GetCameraAffected() const { return m_isCameraAffected; }

		void SetVelocityManager(const std::shared_ptr<VelocityManager>& velocityManager) { m_velocityManager = velocityManager; }
		std::shared_ptr<VelocityManager> GetVelocityManager() const { return m_velocityManager.lock(); }

		Vec3 Move(const Vec2& moveDirection);

		void Move(float x, float y) { Move(Vec2(x, y)); }

		bool IsAim() const noexcept { return m_isAim; }

		void SetIsAim(const bool isAim) { m_isAim = isAim; }

		void SetDefaultForward(const Vec3& defaultForward) { m_defaultForward = defaultForward; }

		const Vec3& GetDefaultForward() const { return m_defaultForward; }
	};
}
}