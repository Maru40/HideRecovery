
/*!
@file PlayerAnimationCtrl.h
@brief PlayerAnimationCtrl.h�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class AnimationCtrl : public Component
	{
	public:

		AnimationCtrl(const std::shared_ptr<GameObject>& objPtr) 
			:Component(objPtr)
		{}

		virtual void OnCreate() override {};
		virtual void OnUpdate() override {};
	};

}

//endbasecross