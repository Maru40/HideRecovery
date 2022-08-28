/*!
@file CameraForwardController.h
@brief CameraForwardController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class VirtualCamera;

	//--------------------------------------------------------------------------------------
	/// �J�����̌����̃R���g���[���[
	//--------------------------------------------------------------------------------------
	class CameraForwardController : public Component
	{
		Vec3 m_direction;	//������������

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