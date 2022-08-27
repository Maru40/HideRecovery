
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
#include "MapCursor.h"

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
		SettingAnimationEvent();	//アニメーションイベント設定
	}

	void Teleport::OnUpdate() {

	}

	void Teleport::SettingFieldMap() {
		auto fieldMap = FieldMap::GetInstance();
		fieldMap->SetMapDraw(false);
		fieldMap->GetMapCursor()->SetTarget(GetGameObject());

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
			auto endEvent = [&,fadeManager, animator]() {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
				GetGameObject()->GetComponent<Transform>()->SetPosition(GetTeleportPosition());	//テレポート

				if (fadeManager) {
					fadeManager->FadeStart(FadeType::In);
				}
			};

			//フェード開始イベント
			if (fadeManager) {
				fadeManager->FadeStart(FadeType::Out, endEvent);
			}
				//エフェクトの再生
		};

		//アニメーションイベントの登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::PutItem_Floor,
			nullptr,
			nullptr,
			exit
		);
	}

	void Teleport::StartTeleport() {
		//テレポート場所を設定
		SetTeleportPosition(GetFieldMap()->GetMapCursor()->GetCursorFiledPosition());

		//マップを閉じる
		CloseMap();

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::PutItem_Floor);	//テレポートアニメーション
		}

			//エフェクトの再生
	}

	void Teleport::OpenMap() {
		auto fieldMap = GetFieldMap();
		if (fieldMap->GetMapDraw()) {	//マップが開いているならCloseする。
			CloseMap();
			return;
		}

		fieldMap->GetMapCursor()->SetTarget(GetGameObject());
		fieldMap->SetMapDraw(true);
	}

	void Teleport::CloseMap() {
		auto fieldMap = GetFieldMap();

		fieldMap->SetMapDraw(false);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<FieldMap> Teleport::GetFieldMap() const {
		return m_fieldMap.lock();
	}

	bool Teleport::IsTeleport() const {
		return GetFieldMap()->IsMapDraw();	//現在はマップが開いているなら飛べる。
	}





	//if (PlayerInputer::GetInstance()->IsRightDown()) {
		//	auto param = Builder::VertexPCTParametor(Vec3(100.0f, 100.0f, 0.0f), Vec2(256.0f, 256.0f), L"Point_TX");
		//	auto sprite = GetStage()->AddGameObject<SpriteObject>(param);
		//	auto spriteTrans = sprite->GetComponent<Transform>();

		//	auto rect = GetFieldMap()->GetRect();
		//	auto startPosition = transform->GetPosition();
		//	float xRate = startPosition.x / (rect.width * 0.5f);
		//	float yRate = startPosition.z / (rect.depth * 0.5f);
		//	spriteTrans->SetPosition(Vec3(256.0f * xRate, 512.0f * yRate, 0.0f));
	//}
}