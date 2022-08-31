/*!
@file CameraForwardController.h
@brief CameraForwardControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class VirtualCamera;

	//--------------------------------------------------------------------------------------
	/// カメラの向きのコントローラー
	//--------------------------------------------------------------------------------------
	class CameraForwardController : public Component
	{
		Vec3 m_direction;	//向きたい方向

		std::weak_ptr<VirtualCamera> m_virtualCamera;

	public:
		CameraForwardController(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<VirtualCamera>& virtualCamera);

		void OnCreate() override;
		void OnUpdate2() override;

	private:
		void ForwardUpdate();

	public:
		void SetDirection(const Vec3& direction) noexcept { m_direction = direction; }

		Vec3 GetDirection() const noexcept { return m_direction; }
	};

}