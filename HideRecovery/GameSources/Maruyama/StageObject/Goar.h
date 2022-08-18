/*!
@file Goar.h
@brief Goar�N���X
�S���F�ێR�T��
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