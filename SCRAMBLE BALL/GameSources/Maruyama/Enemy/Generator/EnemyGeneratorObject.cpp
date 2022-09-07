/*!
@file GameManagerObject.cpp
@brief GameManagerObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyGeneratorObject.h"

#include "GeneratorBase.h"
#include "LoadPositionGenerator.h"

#include "GameStageBase.h"
#include "MainStage.h"

#include "Maruyama/Enemy/Object/EnemyObjectBase.h"
#include "Maruyama/Enemy/Object/HeroObject.h"

namespace basecross {
	namespace Enemy {

		EnemyGeneratorObject::EnemyGeneratorObject(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{}

		void EnemyGeneratorObject::OnCreate() {
			CreateLoadPosition();
		}

		void EnemyGeneratorObject::CreateLoadPosition() {
			std::vector<LoadPositionGenerator_Parametor> params = {
				{ L"Hero",        L"EnemyCreatePosition/", L""},
				//{ L"ShotSlime",    L"EnemyCreatePosition/", L""},
				//{ L"BigSlime",     L"EnemyCreatePosition/", L""},
				//{ L"BigShotSlime", L"EnemyCreatePosition/", L""},
			};

			enum class GenerateType {
				Hero,
			};

			auto index = 0;
			for (auto& param : params) {
				param.fileName = MainStage::GetLoadMapName();
				std::shared_ptr<GeneratorBase> generator = nullptr;

				auto type = static_cast<GenerateType>(index);
				switch (type)
				{
				case GenerateType::Hero:
					generator = GetStage()->AddGameObject<GameObject>()->
						AddComponent<LoadPositionGenerator<Enemy::HeroObject>>(param);
					break;
				}

				index++;
			}
		}

	}
}