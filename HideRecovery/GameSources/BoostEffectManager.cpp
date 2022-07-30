/*!
@file BoostEffectManager.cpp
@brief BoostEffectManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BoostEffectManager.h"

#include "BoostEffectObject.h"

#include "LaserBase.h"

#include "PlayerInputer.h"
#include "MaruUtility.h"

namespace basecross {

	BoostEffectManager::BoostEffectManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_effectObject(nullptr)
	{}

	void BoostEffectManager::OnCreate() {
		
	}

	void BoostEffectManager::OnUpdate() {
		if (PlayerInputer::IsRBDown()) {
			CreateEffect();
		}

		if (m_effectObject) {
			for (auto& object : m_effectObject->GetEffectObjects()) {
				//ポジションセット
				auto position = CalculatePosition();
				object->GetComponent<Transform>()->SetPosition(position);
				//Laser設定
				auto laser = object->GetComponent<LaserBaseDraw>(false);
				if (laser) {
					laser->SetForward(-transform->GetForward());
				}
			}
		}
	}

	Vec3 BoostEffectManager::CalculatePosition() {
		auto position = transform->GetPosition();
		auto offset = Vec3(0.0f, 0.0f, 1.5f);
		position += maru::Utility::ConvertForwardOffset(-transform->GetForward(), offset);
		return position;
	}

	void BoostEffectManager::CreateEffect() {
		//生成
		auto position = CalculatePosition();
		auto effect = GetStage()->Instantiate<BoostEffectObject>(position, Quat::Identity());

		for (auto& object : effect->GetEffectObjects()) {
			auto laser = object->GetComponent<LaserBaseDraw>(false);
			if (laser) {
				laser->SetForward(-transform->GetForward());
			}
		}

		m_effectObject = effect;
	}
}