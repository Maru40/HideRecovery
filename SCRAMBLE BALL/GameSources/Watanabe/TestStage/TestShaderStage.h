/*!
@file   TestShaderStage.h
@brief  シェーダー用のテストステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TestShaderStage :public Stage {
		void CreateViewLight();

		shared_ptr<GameObject> CreatePlayer();
		shared_ptr<GameObject> CreateSphere();
		shared_ptr<GameObject> CreateFloor();
	public:
		TestShaderStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
