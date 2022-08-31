#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "../Component/FadeComponent.h"

namespace basecross {
	class TransitionSpriteUI :public UIObjectBase {
	public:
		TransitionSpriteUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override {}

		void FadeIn() {
			// 画面遷移なので逆になる
			// 画面はFadeInに対して、このオブジェクトはFadeOutする
			GetFade()->FadeOut();
		}
		void FadeOut() {
			// 上に同じく逆
			GetFade()->FadeIn();
		}
		shared_ptr<FadeComponent> GetFade() {
			return GetComponent<FadeComponent>();
		}
	};
}
