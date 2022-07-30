#pragma once
#include "stdafx.h"

namespace basecross
{
	// 画面上部のステージセレクトタイトル文字用のクラス
	class StageSelectTitle : public GameObject
	{
	public:
		StageSelectTitle(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}