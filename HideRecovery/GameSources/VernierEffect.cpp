/*!
@file VernierEffect.cpp
@brief VernierEffectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EffectBase.h"
#include "VernierEffect.h"

#include "EffectManager.h"
#include "BoardObject.h"
#include "ParticleManager.h"

#include "MaruUtility.h"

namespace basecross {

	namespace Effect {

		VernierEffect::VernierEffect(const std::shared_ptr<GameObject>& objPtr) :
			Base(objPtr)
		{}

		std::shared_ptr<GameObject> tempObj = nullptr;

		void VernierEffect::OnCreate() {
			auto effectManager = EffectManager::GetInstance(GetStage());
			std::vector<ParticleData> datas = {};

			//右側
			for (auto data : effectManager->GetVernierData()) {
				//オフセットデータ
				data.offsetData.position = Vec3(+0.15f, 0.25f, -0.5f);
				data.offsetData.rotation = Vec3(0.0f, XMConvertToRadians(150.0f), 0.0f);
				data.offsetData.scale = Vec3(1.0f, 2.0f, 1.0f);
				//ループに合わせた変更
				data.particleParam.isLoop = true;
				data.particleParam.intervalTime = 0.25f;
				data.particleParam.parent = GetGameObject();
				datas.push_back(data);
			}

			//左側
			for (auto data : effectManager->GetVernierData()) {
				//オフセットデータ
				data.offsetData.position = Vec3(-0.15f, 0.25f, -0.5f);
				data.offsetData.rotation = Vec3(0.0f, XMConvertToRadians(210.0f), 0.0f);
				//ループに合わせた変更
				data.particleParam.isLoop = true;
				data.particleParam.intervalTime = 0.25f;
				data.particleParam.parent = GetGameObject();
				datas.push_back(data);
			}

			CreateParticles(datas);

			SetEffectsDrawActive(false);

			transform->GetScale();
		}

	}
}