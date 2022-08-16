﻿#pragma once
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

		float m_moveSpeed = 1.0f;

	public:
		ObjectMover(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override {}

		void OnDraw() override {}

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
	};
}
}