/*!
@file MainStage.cpp
@brief MainStage����
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

#include "Itabashi/OnlineTestRoom.h"
#include "Itabashi/OnlinePlayerManager.h"
#include "GameManagerObject.h"
#include "Itabashi/GamePlayerManager.h"

using namespace basecross::Enemy;

namespace basecross {
	//wstring MainStage::sm_loadMapName = L"StageS1_Copy.csv";
	wstring MainStage::sm_loadMapName = L"StageS2.csv";
	//wstring MainStage::sm_loadMapName = L"TestOwnArea.csv";

	void MainStage::CreateViewLight() {
		CreateStartCamera(sm_loadMapName);
		CreateMainCamera();
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

		ChangeMainCamera(); //�����J�����̐ݒ�
	}

	void MainStage::OnCreate() {
		try {
			AddGameObject<EfkInterface>();
			AddGameObject<Debug>();
			Debug::GetInstance()->Log(L"MainStage");
			// �X�J�C�{�b�N�X
			//AddGameObject<Skybox2>();

			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto cameraObj = Instantiate<CameraObject>();

			EventSystem::GetInstance(GetThis<Stage>())->SetBasicInputer(PlayerInputer::GetInstance());
			AddGameObject<GameManagerObject>();

			auto onlineRoom = AddGameObject<Online::OnlineTestRoom>();
			auto tester = onlineRoom->GetComponent<Online::OnlineTester>();
			onlineRoom->AddComponent<GamePlayerManager>();

			//�X�e�[�W�̐ݒ�
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// BGM�̍Đ�

			//Generator�̐���
			//AddGameObject<EnemyGeneratorObject>();

			//Map�̓ǂݍ���
			CreateMap(sm_loadMapName);

			m_gameStartUI = AddGameObject<GameStartUI>();
			m_gameFinishUI = AddGameObject<GameFinishUI>();
			//�B���A�C�e���̐ݒ�
			//Instantiate<HideItemObject>(Vec3(0.0f, 0.0f, 0.0f), Quat::Identity());

			//�f�o�b�O
			//AddGameObject<DebugObject>();

			Debug::GetInstance()->Log(L"����------------------------------");
			Debug::GetInstance()->Log(L"X  : �u��");
			Debug::GetInstance()->Log(L"A  : �E��");
			Debug::GetInstance()->Log(L"B  : �u���t");
			Debug::GetInstance()->Log(L"�� : �u���ꏊ�����؂�ւ�");
			Debug::GetInstance()->Log(L"----------------------------------");
		}
		catch (...) {
			throw;
		}
	}

	void MainStage::OnUpdate() {
		// �f�o�b�O�p
		const auto& keyBoard = App::GetApp()->GetMyInputDevice()->GetKeyBoard();
		if (keyBoard.IsInputDown(KeyCode::Alpha1)) {
			m_gameStartUI->Start();
			Debug::GetInstance()->Log(L"StartUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha2)) {
			m_gameFinishUI->Start();
			Debug::GetInstance()->Log(L"FinishUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha0)) {
			m_gameStartUI->Reset();
			m_gameFinishUI->Reset();
			Debug::GetInstance()->Log(L"ResetUI");
		}
	}

	void MainStage::SetLoadMapName(const wstring& name) {
		sm_loadMapName = name;
	}

	wstring MainStage::GetLoadMapName() {
		return sm_loadMapName;
	}

	std::shared_ptr<GameStartUI> MainStage::GetGameStartUI() {
		return m_gameStartUI;
	}
	std::shared_ptr<GameFinishUI> MainStage::GetGameFinishUI() {
		return m_gameFinishUI;
	}
}
//end basecross