#pragma once
#include "stdafx.h"

namespace basecross {
	class StageClear : public GameObject
	{
		std::shared_ptr<PCTSpriteDraw> m_drawComp;

	public:
		StageClear(const std::shared_ptr<Stage>& stage)
			: GameObject(stage)
		{
		}

		void OnCreate() override;
		void OnDraw() override;
	};
}