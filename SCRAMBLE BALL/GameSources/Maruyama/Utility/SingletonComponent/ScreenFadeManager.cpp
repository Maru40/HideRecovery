/*!
@file ScreenFadeManager.cpp
@brief ScreenFadeManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "ScreenFadeManager.h"

#include "../Component/FadeSprite.h"

#include "Maruyama/UI/2D/Object/SpriteObject.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	std::weak_ptr<ScreenFadeManager>  maru::SingletonComponent<ScreenFadeManager>::sm_instance;

	ScreenFadeManager::ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr)
		:ScreenFadeManager(objPtr, nullptr)
	{}

	ScreenFadeManager::ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<FadeSpriteManager>& fadeSpriteManager)
		:SingletonComponent(objPtr), m_fadeSpriteManager(fadeSpriteManager)
	{}

	void ScreenFadeManager::OnCreate() {
		if (!m_fadeSpriteManager) { //nullptrだったら
			auto param = Builder::VertexPCTParametor(Vec3(1960.0f, 1080.0f, 0.0f), Vec2(256.0f, 256.0f), L"SquareSprite_TX");
			auto spriteObject = GetStage()->AddGameObject<SpriteObject>(param);
			spriteObject->GetComponent<SpriteObject::DrawComp>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
			m_fadeSpriteManager = spriteObject->AddComponent<FadeSpriteManager>();

			const int Layer = 100;
			GetGameObject()->SetDrawLayer(Layer);
			spriteObject->SetDrawLayer(Layer);
		}
	}

	void ScreenFadeManager::FadeStart(const FadeType& type, const std::function<void()>& endFunction, const float& fadeTime) {
		m_fadeSpriteManager->FadeStart(type, endFunction, fadeTime);
	}

	bool ScreenFadeManager::IsEnd() const {
		return m_fadeSpriteManager->IsEnd();
	}

}