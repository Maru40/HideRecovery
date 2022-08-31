/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"
#include "StageFloor.h"
#include "EnergeyGauge.h"
#include "PlayerHpgauge.h"
#include "Reticle.h"
#include "CountTime.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -20.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void GameStage::OnCreate() {
		GameStageBase::OnCreate();
		try {
			//ビューとライトの作成
			CreateViewLight();
		}
		catch (...) {
			throw;
		}

		// ゲームオブジェクトの追加
		
		// プレイヤー（立方体）の表示
		auto player = AddGameObject<Player>();
		// 地面（直方体）の表示
		AddGameObject<StageFloor>();
		// エネルギーゲージの追加
		AddGameObject<EnergeyGauge>();
		// プレイヤーのHPゲージの追加
		AddGameObject<PlayerHpgauge>();
		// レティクルの追加
		AddGameObject<Reticle>(player);
		//// 経過時間の追加
		//AddGameObject<CountTime>();
	}

}
//end basecross