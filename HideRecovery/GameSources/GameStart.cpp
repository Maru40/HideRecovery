/*!
@file GameStart.cpp
@brief 開始演出など実体
*/

#include "stdafx.h"
#include "GameStart.h"

#include "GameStage.h"
#include "PolygonTexture.h"
#include "TimeManager.h"
#include "Freeze.h"
#include "ChangeCameraManager.h"

#include "GameManager.h"

namespace basecross
{
	void GameStart::OnCreate()
	{
		// ステージに板ポリを追加
		auto stage = GetStage();
		m_Go = stage->AddGameObject<PolygonTexture>(UiData(L"Go", Vec3(0.0f, 0.0f, 0.0f), Vec3(256.0f, 128.0f, 0.0f), Vec2(0.0f, 0.0f)));
		m_Ready = stage->AddGameObject<PolygonTexture>(UiData(L"Ready", Vec3(0.0f, -500.0f, 0.0f), Vec3(512.0f, 128.0f, 0.0f), Vec2(1.0f, 1.0f)));

		m_ReadyTrans = m_Ready->GetComponent<Transform>();
		m_GoTrans = m_Go->GetComponent<Transform>();

		// タイムマネージャコンポーネントを追加
		m_TimerComp = AddComponent<TimeManager>();
	}

	void GameStart::OnUpdate()
	{
		auto readyPos = m_ReadyTrans->GetPosition();
		if (readyPos.y <= m_ReadyMaxPosY) //Readyのポジションが終点じゃないならReadyを動かす
		{
			Ready();
		}
		else // Readyが終点に着いたらGoを大きくする
		{
			Go();
		}
	}

	void GameStart::Ready()
	{
		ChangeCameraManager::GetInstance()->SetUpdateActive(false); //ターゲットカメラを使えなくする

		auto delta = App::GetApp()->GetElapsedTime(); // デルタ時間を取得
		auto readyPos = m_ReadyTrans->GetPosition(); // ポジションを取得

		// ポジションの更新
		auto newPos = readyPos;
		newPos.y += m_ReadyMoveSpeed * delta;
		m_ReadyTrans->SetPosition(newPos);
	}

	void GameStart::Go()
	{
		auto delta = App::GetApp()->GetElapsedTime(); //デルタ時間を取得
		auto goScale = m_GoTrans->GetScale(); // スケールを取得

		if (m_GoMaxScale.x >= goScale.x)
		{
			//最大まで大きくなっていないなら大きくする
			auto newScale = goScale + Vec3(m_GoGrowthSpeed) * delta;
			m_GoTrans->SetScale(newScale);
		}
		else
		{
			auto timer = dynamic_pointer_cast<TimeManager>(m_TimerComp);
			auto data = TimeData(L"End", 2.0f, [&]() {End(); });
			timer->AddTimer(data); // End関数を呼び出すタイマーを定義

			auto time = timer->GetTimeRate(L"End"); //Endのタイムレートを取得

			// アルファ値を下げる
			auto draw = m_Go->GetComponent<PCTSpriteDraw>();
			auto color = draw->GetDiffuse();
			color.w = time;
			draw->SetDiffuse(color);
		}
	}

	void GameStart::End()
	{
		// ステージに追加されているオブジェクト全ての更新処理を再開する
		auto objes = GetStage()->GetGameObjectVec(); // ステージに配置されているオブジェクトを取得
		for (auto obj : objes)
		{
			auto isFreeze = obj->GetComponent<Freeze>(false);
			if (isFreeze) // Freezeコンポーネントを持っていたら
			{
				obj->SetUpdateActive(true); // 更新を再開
			}
		}

		ChangeCameraManager::GetInstance()->SetUpdateActive(true); //ターゲットカメラを使えるようにする

		// Readyを消去
		m_Ready->SetDrawActive(false);
		m_Ready->SetUpdateActive(false);

		// Goを消去
		m_Go->SetDrawActive(false);
		m_Go->SetUpdateActive(false);

		// 自身を消去
		this->SetDrawActive(false);
		this->SetUpdateActive(false);

		// ゲームをプレイ中に遷移
		GameManager::GetInstance()->ChangeState(GameManager::State::Game);
	}
}