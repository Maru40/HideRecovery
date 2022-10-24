#pragma once
#include "stdafx.h"
#include "GameStageBase.h"

namespace basecross
{
	/// <summary>
	/// オンラインマッチ用ステージ
	/// </summary>
	class OnlineMatchStage : public GameStageBase
	{
	public:
		void OnCreate() override;

		void OnUpdate() override;
	};
}