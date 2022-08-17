#pragma once
#include "stdafx.h"

namespace basecross {
	class NumberSprite :public GameObject {
		int m_number;
		vector<VertexPositionColorTexture> vertices;
	public:
		NumberSprite(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void SetValue(int number);
	};

	class Numbers :public GameObject {
		vector<shared_ptr<NumberSprite>> m_numbers;
		int m_numDigits;
	public:
		Numbers(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void SetNumber(int num);
	};
}
