/*!
@file   TestShaderStage.h
@brief  シェーダー用のテストステージ
*/

#pragma once
#include "stdafx.h"
#include "../Stage/AdvStageBase.h"

namespace basecross {
	class TestShaderStage :public AdvStageBase {
		void CreateViewLight();

		shared_ptr<GameObject> CreatePlayer();
		shared_ptr<GameObject> CreateSphere();
		shared_ptr<GameObject> CreateFloor();
	public:
		TestShaderStage() :AdvStageBase() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
