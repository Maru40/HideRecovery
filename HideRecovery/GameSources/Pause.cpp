/*!
@file Pause.cpp
@brief ポーズ画面など実体
*/

#include "stdafx.h"
#include "Scene.h"
#include "Pause.h"

#include "PolygonTexture.h"
#include "Cursor.h"

#include "GameStage.h"
#include "SoundManager.h"

namespace basecross
{
	void Pause::OnCreate()
	{
		// 板ポリの定義
		Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-500.0f, +500.0f, 0.0f), color, Vec2(0.0f, 0.0f)},
			{Vec3(+500.0f, +500.0f, 0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(-500.0f, -500.0f, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(+500.0f, -500.0f, 0.0f), color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		DeployUi(); //Uiの配置

		// 描画の設定
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"PauseBack"); //テクスチャの設定

		// トランスフォームの設定
		auto transComp = GetComponent<Transform>();
		Vec3 pos(0.0f, 0.0f, 0.0f);
		transComp->SetScale(0.6f, 0.6f, 1.0f);
		transComp->SetPosition(pos);

		SetAlphaActive(true); // 透明度を適用
		SetDrawActive(false); // 描画をしない

		SetDrawLayer(0); // 描画レイヤーの設定
	}

	void Pause::OnUpdate()
	{
		//コントローラーの取得
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_START && !m_IsGameOver) // スタートボタンが押されたら
		{
			SetPause(); // ポーズさせる
		}

		// カーソルの移動
		MoveCursor();
	}

	void Pause::DeployUi()
	{
		auto stage = GetStage(); // ステージを取得

		// ポーズ画面用のカーソルの配置
		m_Cursor = stage->AddGameObject<Cursor>();
		for (auto uiData : pauseDatas)
		{
			m_Cursor->AddUiTexture(uiData); //UIの配置
		}
		m_Cursor->SetCursorPos(0);
		m_Cursor->SetAllDrawActive(false); //描画を消す
		m_Cursor->SetAllDrawLayer(2); //描画するレイヤーの設定
	}

	void Pause::SetPause()
	{
		auto setValue = IsDrawActive(); //描画されているかを取得
		SetDrawActive(!setValue); //描画状態を反転
		m_Cursor->SetAllDrawActive(!setValue); //カーソルの描画状態を反転
		m_Cursor->SetCursorPos(-1); //カーソルの位置を一番上に

		Pausing(setValue); //時間停止させる
	}

	void Pause::GameOver()
	{
		m_Cursor->ClearUITexture(); // Pause画面のUIを削除
		// UIの配置
		for (auto uiData : gameOverDatas)
		{
			m_Cursor->AddUiTexture(uiData); //UIの配置
		}
		m_Cursor->SetCursorPos(0);
		m_Cursor->SetAllDrawActive(false); //描画を消す
		m_Cursor->SetAllDrawLayer(2); //描画するレイヤーの設定

		auto setValue = IsDrawActive(); //描画されているかを取得
		SetDrawActive(!setValue); //描画状態を反転
		m_Cursor->SetAllDrawActive(!setValue); //カーソルの描画状態を反転
		Pausing(setValue); //時間停止させる
		m_IsGameOver = true;
	}

	void Pause::Pausing(bool value)
	{
		auto stageObj = GetGameStage()->GetGameObjectVec(); //ステージに配置されているオブジェクトの取得
		for (auto obj : stageObj)
		{
			auto poseUi = dynamic_pointer_cast<Pause>(obj);
			if (!poseUi) //Pauseじゃなかったら
			{
				obj->SetUpdateActive(value); //更新を止める
			}
		}

	}

	void Pause::MoveCursor()
	{
		if (GetDrawActive())
		{
			//コントローラーの取得
			auto& app = App::GetApp();
			auto device = app->GetInputDevice();
			const auto& pad = device.GetControlerVec()[0];

			// カーソルを上に移動
			if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				// SEを再生
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
				}

				// ポーズ時
				m_Cursor->MoveCursor(-1);
			}

			// 下に移動
			if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				// SEを再生
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
				}

				// ポーズ時
				m_Cursor->MoveCursor(+1);
			}

			//コマンドの実行
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				// SEを再生
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::EnterSE, 0.15f);
				}

				// ポーズ時
				m_Cursor->Select();
			}
		}
	}
}