/*!
@file GameManagerObject.cpp
@brief GameManagerObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "GameManagerObject.h"

#include "../SingletonComponent/GameManager.h"
#include "SlowTimerManager.h"
#include "SoundManager.h"
#include "HitStopManager.h"

#include "SingletonComponent.h"
#include "ImpactMap.h"

#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"

#include "PlayerInputer.h"
#include "Maruyama/Utility/Component/FadeSprite.h"

#include "Maruyama/Effect/Component/EffectManager.h"

#include "Maruyama/Utility/SingletonComponent/ScreenFadeManager.h"

#include "TaskList.h"
#include "Maruyama/Player/Component/FieldMap.h"

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
		AddComponent<FieldMap>();
	}

}

//endbasecross