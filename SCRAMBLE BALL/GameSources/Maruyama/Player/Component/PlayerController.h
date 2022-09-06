
/*!
@file PlayerController.h
@brief PlayerController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class RotationController;

	//--------------------------------------------------------------------------------------
	/// �v���C���[�R���g���[���[
	//--------------------------------------------------------------------------------------
	class PlayerController : public Component
	{
		std::weak_ptr<RotationController> m_rotationController;

	public:
		PlayerController(const std::shared_ptr<GameObject>& objPtr);

		void OnStart() override;
		void OnUpdate() override;

	private:
		void Rotation();
	};

}