#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageComplete : public GameObject
	{
	public:
		StageComplete(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}