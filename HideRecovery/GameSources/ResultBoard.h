#pragma once
#include "stdafx.h"

namespace basecross
{
	class ResultBoard : public GameObject
	{
		public:
			ResultBoard(const std::shared_ptr<Stage>&stage)
				: GameObject(stage)
			{
			}
			void OnCreate() override;
			void OnDraw() override;
	};
}