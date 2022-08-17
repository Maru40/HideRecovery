#include "stdafx.h"
#include "Numbers.h"

namespace basecross {
	NumberSprite::NumberSprite(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_number(0)
	{}

	void NumberSprite::OnCreate() {
	}

	void NumberSprite::SetValue(int number) {
	}

	Numbers::Numbers(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void Numbers::OnCreate() {
	}
	void Numbers::SetNumber(int num) {
	}
}