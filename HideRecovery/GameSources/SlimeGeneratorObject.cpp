
/*!
@file SlimeGeneratorObject.cpp
@brief SlimeGeneratorObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "NormalSlimeObject.h"
#include "ShootSlimeObject.h"
#include "EvadeSlimeObject.h"
#include "BigSlimeObject.h"

#include "SlimeGeneratorObject.h"
#include "RandomPositionGenerator.h"

#include "LoadPositionGenerator.h"

#include "MainStage.h"

#include "BigShootSlimeObject.h"


namespace basecross {

	SlimeGeneratorObject::SlimeGeneratorObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void SlimeGeneratorObject::OnCreate() {
		CreateNormalSlimeGenerator();
		CreateShootSlimeGenerator();
		CreateEscapeSlimeGenerator();
		CreateBigSlimeGenerator();

		CreateLoadPostitionGenerator();
	}

	void SlimeGeneratorObject::CreateNormalSlimeGenerator(){
		auto parametor = RandomPositionGenerator_Parametor();
		parametor.numCreate = 0;

		auto generator = GetStage()->AddGameObject<GameObject>()->AddComponent<RandomPositionGenerator<Enemy::NormalSlimeObject>>(parametor);
	}

	void SlimeGeneratorObject::CreateShootSlimeGenerator() {
		auto parametor = RandomPositionGenerator_Parametor();
		parametor.numCreate = 0;

		GetStage()->AddGameObject<GameObject>()->AddComponent<RandomPositionGenerator<Enemy::ShootSlime::ShootSlimeObject>>(parametor);
	}

	void SlimeGeneratorObject::CreateEscapeSlimeGenerator() {
		auto parametor = RandomPositionGenerator_Parametor();
		parametor.numCreate = 0;

		GetStage()->AddGameObject<GameObject>()->AddComponent<RandomPositionGenerator<Enemy::EvadeSlime::EvadeSlimeObject>>(parametor);
	}

	void SlimeGeneratorObject::CreateBigSlimeGenerator() {
		auto parametor = RandomPositionGenerator_Parametor();
		parametor.numCreate = 1;

		//GetStage()->AddGameObject<GameObject>()->AddComponent<RandomPositionGenerator<Enemy::BigSlime::BigSlimeObject>>(parametor);
		//GetStage()->AddGameObject<GameObject>()->AddComponent<RandomPositionGenerator<Enemy::BigShootSlime::BigShootSlimeObject>>(parametor);
	}

	void SlimeGeneratorObject::CreateLoadPostitionGenerator() {
		std::vector<LoadPositionGenerator_Parametor> params = {
			{ L"Slime",        L"EnemyCreatePosition/", L""},
			{ L"ShotSlime",    L"EnemyCreatePosition/", L""},
			{ L"BigSlime",     L"EnemyCreatePosition/", L""},
			{ L"BigShotSlime", L"EnemyCreatePosition/", L""},
		};

		enum class GenerateType{
			Normal,
			Shoot,
			BigNormal,
			BigShot,
		};

		auto index = 0;
		for (auto& param : params) {
			param.fileName = MainStage::GetLoadMapName();
			std::shared_ptr<GeneratorBase> generator = nullptr;

			auto type = static_cast<GenerateType>(index);
			switch (type)
			{
			case GenerateType::Normal:
				generator = GetStage()->AddGameObject<GameObject>()->AddComponent<LoadPositionGenerator<Enemy::NormalSlimeObject>>(param);
				break;

			case GenerateType::Shoot:
				generator = GetStage()->AddGameObject<GameObject>()->AddComponent<LoadPositionGenerator<Enemy::ShootSlime::ShootSlimeObject>>(param);
				break;

			case GenerateType::BigNormal:
				generator = GetStage()->AddGameObject<GameObject>()->AddComponent<LoadPositionGenerator<Enemy::BigSlime::BigSlimeObject>>(param);
				break;

			case GenerateType::BigShot:
				generator = GetStage()->AddGameObject<GameObject>()->AddComponent<LoadPositionGenerator<Enemy::BigShootSlime::BigShootSlimeObject>>(param);
				break;
			}

			index++;
		}
	}

}

//endbasecross