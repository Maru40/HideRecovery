/*!
@file StageSelectSpriteManager.cpp
@brief StageSelectSpriteManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "StageSelectSpriteManager.h"

#include "BuilderVertexPCT.h"
#include "SpriteObject.h"
#include "SelectSprite.h"
#include "StageSelectSpriteObject.h"

#include "LBRBSpriteObject.h"

#include "MoveUI.h"

#include "PlayerInputer.h"

#include "SelecterSprite.h"

#include "SingletonComponent.h"
#include "FadeSprite.h"
#include "SoundManager.h"

#include "MainStage.h"

#include "ScreenFadeManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	StageSelectSpriteManager_Parametor::StageSelectSpriteManager_Parametor()
		:StageSelectSpriteManager_Parametor(0.0f, 1.0f, Vec3(0.0f, 0.0f, 0.0f))
	{}

	StageSelectSpriteManager_Parametor::StageSelectSpriteManager_Parametor(const float& moveRange, const float& moveSpeed, const Vec3& moveDirect)
		:moveRange(moveRange), moveSpeed(moveSpeed), moveDirect(moveDirect)
	{}

	//--------------------------------------------------------------------------------------
	/// ステージセレクトスプライト管理本体
	//--------------------------------------------------------------------------------------

	StageSelectSpriteManager::StageSelectSpriteManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_param(Parametor())
	{}

	void StageSelectSpriteManager::OnCreate() {
		CreateStageSelectSprites();
		//CreateRBAndLB();

		CreateSelecterSprite();
	}

	void StageSelectSpriteManager::OnUpdate() {
		return;
		if (PlayerInputer::IsRBDown()) {
			m_RBSelect->DecisionInvoke();
		}

		if (PlayerInputer::IsLBDown()) {
			m_LBSelect->DecisionInvoke();
		}
	}

	struct SelectSpriteData {
		Vec3 position;
		wstring stageName;
		wstring stageTexture;

		SelectSpriteData(const Vec2& position, const wstring& stageName, const wstring& stageTexture)
			:SelectSpriteData(Vec3(position.x, position.y, 0.0f), stageName, stageTexture)
		{}

		SelectSpriteData(const Vec3& position, const wstring& stageName, const wstring& stageTexture)
			:position(position), stageName(stageName), stageTexture(stageTexture)
		{}
	};

	std::vector<SelectSpriteData> CreateSelectSpriteDatas() {
		std::vector<SelectSpriteData> datas;

		//データ生成
		wstring names[] = {
			L"StageS1.csv",
			L"StageS2.csv",
			L"StageM1.csv",
			L"StageM2.csv",
			L"StageL1.csv",
			L"StageL2.csv",
		};
		
		wstring textureNames[] = {
			L"StageS1_TX",
			L"StageS2_TX",
			L"StageM1_TX",
			L"StageM2_TX",
			L"StageL1_TX",
			L"StageL2_TX",
		};

		Vec2 startPosition(-350.0f, 80.0f);
		Vec2 offsetBase(350.0f, 0.0f);
		float offsetY = -300.0f;

		int index = 0;
		int texutreIndex = 0;
		for (auto name : names) {
			auto offset = offsetBase;
			offset.x *= (float)index++;
			auto position = startPosition + (offset);
			auto data = SelectSpriteData(position, name, textureNames[texutreIndex++]);
			datas.push_back(data);

			auto excess = index % 3;
			if (excess == 0) {
				offsetBase.y += offsetY;
				index = 0;
			}
		}

		return datas;
	}

	void StageSelectSpriteManager::CreateStageSelectSprites() {
		GetStage()->AddGameObject<GameObject>()->AddComponent<ScreenFadeManager>();
		auto datas = CreateSelectSpriteDatas();

		int index = 1;
		for (const auto& data : datas) {
			auto object = GetStage()->AddGameObject<StageSelectSpriteObject>(index++, data.stageTexture);
			auto selectSprite = object->GetComponent<SelectSprite>();

			object->GetComponent<Transform>()->SetPosition(data.position);
			//セレクト時に呼び出す関数
			//フェードアウトしてステージを呼び出す。
			selectSprite->AddDecisionAction([&, data]() { 
				SoundManager::GetInstance(GetStage())->PlayOneShot(SoundManager::ID::EnterSE);
				MainStage::SetLoadMapName(data.stageName);
				ScreenFadeManager::GetInstance()->FadeStart(
					FadeType::Out,
					[&, this]() { 
						SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::SelectBGM);
						PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage"); 
					}
				);}
			);

			m_selectSprites.push_back(selectSprite);
		}
	}

	void StageSelectSpriteManager::CreateRBAndLB() {
		struct Data {
			Vec3 position;
			LBRBSpriteObject::Type type;

			Data(const Vec2& position, const LBRBSpriteObject::Type& type)
				:Data(Vec3(position.x, position.y, 0.0f), type)
			{}
			Data(const Vec3& position, const LBRBSpriteObject::Type& type)
				:position(position), type(type)
			{}
		};

		Data datas[] = {
			Data(Vec2(-550.0f, -70.0f), LBRBSpriteObject::Type::LB),
			Data(Vec2(+550.0f, -70.0f), LBRBSpriteObject::Type::RB),
		};
		
		for (const auto& data : datas) {
			auto object = GetStage()->AddGameObject<LBRBSpriteObject>(data.type);
			object->GetComponent<Transform>()->SetPosition(data.position);

			auto select = object->GetComponent<SelectSprite>();

			switch (data.type)
			{
			case LBRBSpriteObject::Type::LB:
				m_LBSelect = select;
				select->AddDecisionAction([&]() { StartMove(-Vec3::Right()); });
				break;
			case LBRBSpriteObject::Type::RB:
				m_RBSelect = select;
				select->AddDecisionAction([&]() { StartMove( Vec3::Right()); });
				break;
			}
		}
	}

	void StageSelectSpriteManager::CreateSelecterSprite() {
		auto object = GetStage()->AddGameObject<GameObject>();
		auto selecter = object->AddComponent<SelecterSprite>();
		selecter->SetTargets(m_selectSprites);
	}

	void StageSelectSpriteManager::StartMove(const Vec3& direct) {
		m_param.moveDirect = direct;
	}

}