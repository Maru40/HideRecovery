/*!
@file MarchingTesetObject.h
@brief MarchingTesetObject
’S“–ÒFŠÛR —TŠì
*/

#pragma once

#include "stdafx.h"

#include "BcPNTMarchingDraw.h"

namespace basecross {

	class MarchingTesterObject : public GameObject
	{
	public:
		MarchingTesterObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override {
			auto draw = AddComponent<MarchingCube::BcPNTMarchingDraw>();
			draw->SetMeshResource(L"DEFAULT_SPHERE");
			//draw->SetTextureResource(L"gauge");

			
			SetAlphaActive(true);
		}
	};

}