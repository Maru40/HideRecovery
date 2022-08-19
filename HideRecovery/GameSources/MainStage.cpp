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

#include "EnemyGeneratorObject.h"

#include "DebugNumber.h"
#include "NumbersObject.h"
#include "NumbersCtrl.h"

#include "CameraHelper.h"

#include "HideItemObject.h"

#include "HeroPlayerObject.h"
#include "VillainPlayerObject.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/Effekseer/EfkEffect.h"

using namespace basecross::Enemy;

namespace basecross {
	//wstring MainStage::sm_loadMapName = L"StageS1_Copy.csv";
	//wstring MainStage::sm_loadMapName = L"StageS2.csv";
	wstring MainStage::sm_loadMapName = L"TestOwnArea.csv";

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
			AddGameObject<EfkInterface>();
			AddGameObject<Debug>();
			Debug::GetInstance()->Log(L"MainStage");
			// スカイボックス
			//AddGameObject<Skybox2>();

			//ビューとライトの作成
			CreateViewLight();

			auto cameraObj = Instantiate<CameraObject>();

			GameStageBase::OnCreate();

			//ステージの設定
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// BGMの再生

			//Generatorの生成
			//AddGameObject<EnemyGeneratorObject>();

			//Mapの読み込み
			CreateMap(sm_loadMapName);

			//隠すアイテムの設定
			//Instantiate<HideItemObject>(Vec3(0.0f, 0.0f, 0.0f), Quat::Identity());

			//デバッグ
			//AddGameObject<DebugObject>();

			Debug::GetInstance()->Log(L"入力------------------------------");
			Debug::GetInstance()->Log(L"X  : 置く");
			Debug::GetInstance()->Log(L"A  : 拾う");
			Debug::GetInstance()->Log(L"B  : ブラフ");
			Debug::GetInstance()->Log(L"↓ : 置く場所制限切り替え");
			Debug::GetInstance()->Log(L"----------------------------------");
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