
/*!
@file Teleport.cpp
@brief Teleportクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Teleport.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "SingletonComponent.h"
#include "ScreenFadeManager.h"
#include "FadeSprite.h"

#include "FieldMap.h"

#include "Watanabe/DebugClass/Debug.h"
#include "PlayerInputer.h"
#include "SpriteObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// テレポート本体
	//--------------------------------------------------------------------------------------

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor())
	{}

	void Teleport::OnLateStart() {
		SettingFieldMap();			//マップテクスチャの設定
		//SettingAnimationEvent();	//アニメーションイベント設定
	}

	void Teleport::OnUpdate() {
		//Debug::GetInstance()->Log(transform->GetPosition());
		if (PlayerInputer::GetInstance()->IsRightDown()) {
			auto param = Builder::VertexPCTParametor(Vec3(100.0f, 100.0f, 0.0f), Vec2(256.0f, 256.0f), L"Point_TX");
			auto sprite = GetStage()->AddGameObject<SpriteObject>(param);
			auto spriteTrans = sprite->GetComponent<Transform>();

			auto rect = GetFieldMap()->GetRect();
			auto startPosition = transform->GetPosition();
			float xRate = startPosition.x / (rect.width * 0.5f);
			float yRate = startPosition.z / (rect.depth * 0.5f);
			spriteTrans->SetPosition(Vec3(256.0f * xRate, 512.0f * yRate, 0.0f));
		}

		if (PlayerInputer::GetInstance()->IsLeftDown()) {
			//GetFieldMap()->SetMapDraw(GetF);
		}
	}

	void Teleport::SettingFieldMap() {
		auto fieldMap = FieldMap::GetInstance();
		fieldMap->SetMapDraw(false);

		m_fieldMap = fieldMap;
	}

	void Teleport::SettingAnimationEvent() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		//アニメーション終了時に呼ぶイベント
		auto exit = [&, animator]() {
			auto fadeManager = ScreenFadeManager::GetInstance(GetStage());

			//フェード終了イベント
			auto endEvent = [fadeManager]() {
				if (fadeManager) {
					fadeManager->FadeStart(FadeType::In);
				}
			};

			//フェード開始イベント
			if (fadeManager) {
				fadeManager->FadeStart(FadeType::Out, endEvent);
			}

			transform->SetPosition(GetTeleportPosition());	//テレポート

			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);	//アニメーションの再生(将来的に変更)
				//エフェクトの再生
		};

		//アニメーションイベントの登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Dash,
			nullptr,
			nullptr,
			exit
		);
	}

	void Teleport::StartTeleport() {
		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				//テレポートアニメーション
		}

			//エフェクトの再生
	}

	void Teleport::OpenMap() {
		GetFieldMap()->SetMapDraw(true);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<FieldMap> Teleport::GetFieldMap() const {
		return m_fieldMap.lock();
	}
}