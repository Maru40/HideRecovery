/*!
@file Goar.h
@brief Goarクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Goar : public Component
	{
	public:
		Goar(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
	};

}