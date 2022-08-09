
/*!
@file PlayerAnimationCtrl.h
@brief PlayerAnimationCtrl.hクラス
担当：丸山裕喜
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