/*!
@file FadeSpriteManager.cpp
@brief FadeSpriteManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "FadeSprite.h"

#include "SpriteObject.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	FadeSpriteManager::FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr)
		:FadeSpriteManager(objPtr, nullptr)
	{}

	FadeSpriteManager::FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<SpriteObject>& spriteObject)
		: Component(objPtr), m_fadeSprite(spriteObject), m_timer(new GameTimer(0.0f))
	{}

	void FadeSpriteManager::OnCreate() {
		//FadeSpriteの生成
		if (!m_fadeSprite) {
			auto param = Builder::VertexPCTParametor(Vec3(1960.0f, 1080.0f, 0.0f), Vec2(256.0f, 256.0f), L"SquareSprite_TX");
			m_fadeSprite = GetStage()->AddGameObject<SpriteObject>(param);
			m_fadeSprite->GetComponent<SpriteObject::DrawComp>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));

			const int Layer = 100;
			GetGameObject()->SetDrawLayer(Layer);
			m_fadeSprite->SetDrawLayer(Layer);
		}
	}

	void FadeSpriteManager::OnUpdate() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		FadeUpdate();
	}

	void FadeSpriteManager::FadeUpdate() {
		switch (m_type)
		{
		case basecross::FadeType::In:
			UpdateAlpha(m_timer->GetIntervalTimeRate());
			break;
		case basecross::FadeType::Out:
			UpdateAlpha(m_timer->GetTimeRate());
			break;
		}

		m_timer->UpdateTimer();
		if (m_timer->IsTimeUp()) {
			FadeEnd();
		}
	}

	void FadeSpriteManager::UpdateAlpha(const float& alpha) {
		auto draw = m_fadeSprite->GetComponent<SpriteObject::DrawComp>(false);
		auto diffuse = draw->GetDiffuse();
		diffuse.w = alpha;
		draw->SetDiffuse(diffuse);
	}

	void FadeSpriteManager::FadeEnd() {
		if (m_endFunction) {
			m_endFunction();
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void FadeSpriteManager::FadeStart(const FadeType& type, const std::function<void()>& endFunction, const float& fadeTime) {
		if (!m_timer->IsTimeUp()) {
			return;
		}

		m_type = type;
		auto draw = m_fadeSprite->GetComponent<SpriteObject::DrawComp>();
		auto diffuse = draw->GetDiffuse();

		switch (type)
		{
		case basecross::FadeType::In:
			diffuse.w = 1.0f;
			break;
		case basecross::FadeType::Out:
			diffuse.w = 0.0f;
			break;
		}

		draw->SetDiffuse(diffuse);
		m_fadeSprite->SetDrawActive(true);
		m_timer->ResetTimer(fadeTime);
		m_endFunction = endFunction;
	}

	bool FadeSpriteManager::IsEnd() const {
		return m_timer->IsTimeUp();
	}

}