#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleBoard : public GameObject
	{
	public:
		TitleBoard(const std::shared_ptr<Stage>& stage)
			: GameObject(stage)
		{
		}
		void OnCreate() override;
		void OnDraw() override;
	};
}