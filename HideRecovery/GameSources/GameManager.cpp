
/*!
@file RandomPositionGenerator.cpp
@brief RandomPositionGeneratorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "GeneratorBase.h"
#include "GameManager.h"

#include "MaruUtility.h"

#include "DebugObject.h"

#include "SoundManager.h"

#include "TaskList.h"

namespace basecross {

	std::weak_ptr<GameManager>  maru::SingletonComponent<GameManager>::sm_instance;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	struct GameManager::Parametor
	{
		State state = State::Start;
	};

	//--------------------------------------------------------------------------------------
	/// ゲームマネージャー本体
	//--------------------------------------------------------------------------------------

	GameManager::GameManager(const std::shared_ptr<GameObject>& objPtr)
		:maru::SingletonComponent<GameManager>(objPtr), m_paramPtr(new Parametor())
	{}

	void GameManager::OnCreate() {
		ChangeState(State::Game);
	}

	void GameManager::OnUpdate() {
		switch (m_paramPtr->state)
		{
		case State::Start:

			break;

		case State::Game:
			CheckClear();
			break;

		case State::Clear:

			break;

		case State::Result:

			break;

		case State::GameOver:

			break;
		}
	}

	bool GameManager::IsGameClear() {
		if (m_generators.size() == 0) {
			m_generators = maru::Utility::FindExWeakPtrComponents<GeneratorBase>(GetStage());
			return false;
		}

		int count = 0;
		for (const auto& generator : m_generators) {
			count += generator->GetNumObject();
		}

		return count == 0;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void GameManager::ChangeState(const State state) {
		if (m_paramPtr->state == state) {
			return;
		}

		m_paramPtr->state = state;

		switch (state)
		{
		case State::Start:

			break;

		case State::Game:
			break;

		case State::Clear:
			break;

		case State::Result:

			break;

		case State::GameOver:
			//auto pause = maru::Utility::FindGameObject<Pause>();
			//if (pause) {
			//	pause->GameOver();
			//}
			
			break;
		}
	}

	GameManager::State GameManager::GetCurrentState() const {
		return m_paramPtr->state;
	}

	void GameManager::CheckClear() {
		if (IsGameClear()) {
			ChangeState(State::Clear);
		}
	}

}

//endbasecross