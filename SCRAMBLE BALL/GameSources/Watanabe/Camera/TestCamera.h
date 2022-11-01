#pragma once
#include "stdafx.h"
#include "../Utility/GameController.h"

namespace basecross {
	class GameCamera :public Camera {
		// コントローラー
		GameController m_controller;
		// 追従するターゲット
		shared_ptr<GameObject> m_target;
		// カメラの回り込み角度（ラジアン）
		Vec2 m_angleRad;
		// カメラの注視点との距離
		float m_length;
		// カメラの移動速度
		float m_moveSpeed;
		// 感度（1がデフォルト）
		Vec2 m_sensitivity;
		// 上下方向の最大角度
		float m_upDownMaxAngle;
		// 上下方向の最小角度
		float m_upDownMinAngle;
		// 注視点とのオフセット
		Vec3 m_atOffset;
	public:
		GameCamera(const shared_ptr<GameObject>& target,
			const Vec2& defaultAngle =
			Utility::ConvertDegVecToRadVec(Vec2(90, 120)));
		GameCamera(const Vec2& defaultAngle =
			Utility::ConvertDegVecToRadVec(Vec2(90, 120)));

		void OnCreate()override;
		void OnUpdate()override;

		void SetTarget(const shared_ptr<GameObject>& target);

		void SetLength(float length);
		float GetLength() { return m_length; }
	};
}
