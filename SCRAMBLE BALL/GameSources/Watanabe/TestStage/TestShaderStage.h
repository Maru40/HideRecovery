/*!
@file   TestShaderStage.h
@brief  シェーダー用のテストステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TestShaderStage :public Stage {
		void CreateViewLight();
	public:
		TestShaderStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
