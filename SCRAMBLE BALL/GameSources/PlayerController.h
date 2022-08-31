
/*!
@file PlayerController.h
@brief PlayerControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class RotationController;

	//--------------------------------------------------------------------------------------
	/// プレイヤーコントローラー
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