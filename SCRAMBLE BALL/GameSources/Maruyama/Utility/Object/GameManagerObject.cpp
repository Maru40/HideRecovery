/*!
@file GameManagerObject.cpp
@brief GameManagerObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "GameManagerObject.h"

#include "../SingletonComponent/GameManager.h"
#include "../SingletonComponent/SlowTimerManager.h"
#include "../SingletonComponent/SoundManager.h"
#include "Maruyama/Utility/SingletonComponent/HitStopManager.h"

#include "../SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Component/FadeSprite.h"

#include "Maruyama/Effect/Component/EffectManager.h"

#include "Maruyama/Utility/SingletonComponent/ScreenFadeManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"

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