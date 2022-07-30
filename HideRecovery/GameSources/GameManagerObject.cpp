/*!
@file GameManagerObject.cpp
@brief GameManagerObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "GameManagerObject.h"

#include "GameManager.h"
#include "SlowTimerManager.h"
#include "SoundManager.h"
#include "HitStopManager.h"

#include "SingletonComponent.h"
#include "ImpactMap.h"

#include "EnemyAIDirector.h"

#include "PlayerInputer.h"
#include "FadeSprite.h"

#include "EffectManager.h"

#include "ScreenFadeManager.h"

#include "TaskList.h"

namespace basecross {

	GameManagerObject::GameManagerObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void GameManagerObject::OnCreate() {
		AddComponent<GameManager>();
		AddComponent<SlowTimerManager>();
		AddComponent<SoundManager>();
		AddComponent<HitStopManager>();
		//AddComponent<ImpactMap>();
		AddComponent<Enemy::AIDirector>();
		AddComponent<EffectManager>();
		AddComponent<ScreenFadeManager>();
	}

}

//endbasecross