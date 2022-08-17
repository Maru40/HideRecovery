
/*!
@file Respawner.cpp
@brief Respawnerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Respawner.h"

#include "PlayerSpawnPoint.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "Watanabe/Component/PlayerStatus.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// リスポーンをさせるクラスのパラメータ
	//--------------------------------------------------------------------------------------

	Respawner_Parametor::Respawner_Parametor() : 
		time(2.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// リスポーンをさせるクラス本体
	//--------------------------------------------------------------------------------------

	Respawner::Respawner(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor()),
		m_timer(new GameTimer(0))
	{}

	void Respawner::OnCreate() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		m_timer->UpdateTimer();
	}

	void Respawner::Respawn() {
		if (IsRespawn()) {
			auto timerEndFunction = [&]() { 
				transform->SetPosition(GetSpawnPoint()->GetWorldPosition()); 
				if (auto status = GetGameObject()->GetComponent<PlayerStatus>(false)) {
					status->Respawn();
				}
			};

			m_timer->ResetTimer(m_param.time, timerEndFunction);
		}
	}

	bool Respawner::IsRespawn() {
		return GetSpawnPoint() ? true : false;	//スポーンポイントがあるならtrue
	}

}