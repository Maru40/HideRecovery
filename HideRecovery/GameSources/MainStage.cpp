/*!
@file MainStage.cpp
@brief MainStage実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MainStage.h"
#include "PlayerInputer.h"

#include "EnemyObjectBase.h"
#include "WallObject.h"

#include "DebugObject.h"

#include "OriginalMeshStageObject.h"

#include "ArrowObject.h"

#include "ParticleManager.h"

#include "SoundManager.h"

#include "Mathf.h"

#include "EnemyObjectBase.h"

#include "EffectBase.h"

#include "BoardObject.h"
#include "BillBoard_Ex.h"

#include "AuraObjectBase.h"
#include "AuraBase.h"

#include "EffectManager.h"

#include "EnemyBase.h"

#include "TaskList.h"

#include "DebugNumber.h"
#include "NumbersObject.h"
#include "NumbersCtrl.h"

using namespace basecross::Enemy;

namespace basecross {

	wstring MainStage::sm_loadMapName = L"StageS1_Copy.csv";

	const WORD ResetCommand = XINPUT_GAMEPAD_RIGHT_SHOULDER + XINPUT_GAMEPAD_LEFT_SHOULDER + XINPUT_GAMEPAD_START + XINPUT_GAMEPAD_BACK;

	void MainStage::CreateViewLight() {
		CreateStartCamera(sm_loadMapName);
		CreateMainCamera();
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

		ChangeMainCamera(); //初期カメラの設定
	}

	void MainStage::OnCreate() {
		try {
			GameStageBase::OnCreate();

			//ステージの設定
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// スカイボックス
			//AddGameObject<Skybox2>();

			//ビューとライトの作成
			CreateViewLight();

			// BGMの再生

			//Generatorの生成

			//Mapの読み込み
			auto mapOffset = Vec3(0.0, -0.5f, 0.0f);
			CreateMap(sm_loadMapName, mapOffset);

			//デバッグ
			//AddGameObject<GameObject>()->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");

		}
		catch (...) {
			throw;
		}
	}

	void MainStage::SetLoadMapName(const wstring& name) {
		sm_loadMapName = name;
	}

	wstring MainStage::GetLoadMapName() {
		return sm_loadMapName;
	}

}
//end basecross