#pragma once
#include "stdafx.h"

namespace basecross {
	class Reticle : public GameObject
	{
		std::shared_ptr<GameObject> target; // 照準をくっつける対象のオブジェクト

	public:
		Reticle(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& target)
			: GameObject(stage), target(target) // コンストラクタで対象を指定する
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
	};
}