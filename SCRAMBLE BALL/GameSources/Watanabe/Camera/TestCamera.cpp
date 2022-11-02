#include "stdafx.h"
#include "TestCamera.h"
#include "../DebugClass/Debug.h"
#include "Scene.h"

namespace basecross {
	GameCamera::GameCamera(const shared_ptr<GameObject>& target,
		const Vec2& defaultAngle)
		:m_controller(0), m_angleRad(defaultAngle),
		m_length(7.0f), m_moveSpeed(90.0f), m_target(target),
		m_upDownMaxAngle(XMConvertToRadians(170)),
		m_upDownMinAngle(XMConvertToRadians(60)),
		m_atOffset(Vec3(0, 0.75f, 0)), m_sensitivity(1, 0.75f)
	{}
	GameCamera::GameCamera(const Vec2& defaultAngle)
		: GameCamera(nullptr, defaultAngle)
	{}

	void GameCamera::OnCreate() {
	}

	void GameCamera::OnUpdate() {
		auto& app = App::GetApp(); //アプリケーションオブジェクトを取得

		Vec2 vec(0);
		if (m_controller.GetControler().bConnected) {
			// スティックを取得しVec2に変換
			auto stickVec = m_controller.GetRightStickVec();
			vec = Vec2(stickVec.x, stickVec.z);
		}
		else {
			const auto& key = app->GetInputDevice().GetKeyState();
			if (key.m_bPushKeyTbl[VK_UP]) {
				vec += Vec2(0, 1);
			}
			if (key.m_bPushKeyTbl[VK_DOWN]) {
				vec += Vec2(0, -1);
			}
			if (key.m_bPushKeyTbl[VK_RIGHT]) {
				vec += Vec2(-1, 0);
			}
			if (key.m_bPushKeyTbl[VK_LEFT]) {
				vec += Vec2(1, 0);
			}
		}
		//スティックの傾きに応じてカメラを回り込ませる
		auto degVec = Vec2(vec * m_sensitivity * m_moveSpeed);
		m_angleRad += Utility::ConvertDegVecToRadVec(degVec) * app->GetElapsedTime();

		// 上下方向に制限をかける
		m_angleRad.y = Utility::Clamp(
			m_angleRad.y, m_upDownMinAngle, m_upDownMaxAngle);

		// 球面座標系変換
		Vec3 eye;
		eye.x = m_length * sinf(m_angleRad.y) * cosf(m_angleRad.x);
		eye.y = -m_length * cosf(m_angleRad.y);
		eye.z = -m_length * sinf(m_angleRad.y) * sinf(m_angleRad.x);
		SetEye(eye + GetAt());
		if (auto camObj = GetCameraObject()) {
			camObj->GetComponent<Transform>()->SetPosition(eye + GetAt());
		}

		// ターゲットが設定されている場合
		if (m_target) {
			// 注視点をターゲットの位置に
			auto targetPos = m_target->GetComponent<Transform>()->GetPosition();
			SetAt(targetPos + m_atOffset);
		}
	}

	void GameCamera::SetLength(float length) {
		// 遅れて追従させる
		auto _length = Lerp::CalculateLerp(
			m_length, length,
			0, 1,
			5 * App::GetApp()->GetElapsedTime(),
			Lerp::rate::Linear
		);
		m_length = _length;
	}

	void GameCamera::SetTarget(const shared_ptr<GameObject>& target) {
		m_target = target;
		//auto targetFront = m_target->GetComponent<Transform>()->GetForword();
	}
}