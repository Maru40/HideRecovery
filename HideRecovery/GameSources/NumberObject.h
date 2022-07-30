
/*!
@file NumberObject.h
@brief NumberObjectクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class NumberObject : public GameObject {

	public:

		NumberObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
	};

}

//endbasecross