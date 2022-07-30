
/*!
@file ClearDirection.cpp
@brief ClearDirectionのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"

#include "ClearDirection.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "Task_Wait.h"

#include "SlowTimerManager.h"

#include "SpriteObject.h"
#include "ScalePop.h"
#include "FadeSprite.h"

#include "ClearUIObject.h"

#include "SingletonComponent.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "ScreenFadeManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	ClearDirection_Parametor::ClearDirection_Parametor() :
		slowTime(30.0f), 
		slowTimeSpeed(0.001f),
		beforeInsetUIStartWaitParamPtr(new Task::Wait::Parametor(0.5f, 1.0f, maru::DeltaType::Real)),
		fadeInBackSpriteTime(0.125f),
		insertUIParamPtr(new Task::InsertUI_Parametor(std::make_shared<ScalePop_Parametor>(Vec3(1.0f), Vec3(1.0f), 1.5f))),
		endWaitParamPtr(new Task::Wait_Parametor(1.5f, 1.0f, maru::DeltaType::Real))
	{}

	//--------------------------------------------------------------------------------------
	///	ClearDirection
	//--------------------------------------------------------------------------------------

	ClearDirection::ClearDirection(const std::shared_ptr<GameObject>& objPtr):
		ClearDirection(objPtr, Parametor())
	{}

	ClearDirection::ClearDirection(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
		:Component(objPtr), m_param(parametor), m_taskList(new TaskList<TaskEnum>())
	{}

	void ClearDirection::OnCreate() {
		CreateClearUI();

		for (auto& object : m_clearUI->GetChildren()) {
			object->SetDrawActive(false);
		}

		DefineTask();
	}

	void ClearDirection::OnUpdate() {
		if (m_taskList->IsEnd()) {
			return;
		}

		m_taskList->UpdateTask();
	}

	void ClearDirection::Start() {
		SlowTimerManager::GetInstance()->ForceStop();
		SlowTimerManager::GetInstance()->StartTimer(m_param.slowTime, m_param.slowTimeSpeed);
		SelectTask();
	}

	void ClearDirection::DefineTask() {
		//Wait_BeforeInsertUI
		m_taskList->DefineTask(TaskEnum::Wait_BeforeInsertUI, std::make_shared<Task::Wait>(m_param.beforeInsetUIStartWaitParamPtr));

		//FadeInBackSprite
		m_taskList->DefineTask(TaskEnum::FadeInBackSprite,
			[&]() { m_clearUI->GetBackUI()->SetDrawActive(true);
					m_clearUI->GetBackUI()->AddComponent<FadeSpriteManager>(m_clearUI->GetBackUI())->FadeStart(FadeType::Out, nullptr, m_param.fadeInBackSpriteTime);
			},
			[&]() { return m_clearUI->GetBackUI()->GetComponent<FadeSpriteManager>()->IsEnd(); },
			[]() {  }
		);

		//InsertUI
		m_taskList->DefineTask(TaskEnum::InsertUI, std::make_shared<Task::InsertUI>(m_clearUI->GetClearUI(), m_param.insertUIParamPtr));

		//Wait_End
		m_taskList->DefineTask(TaskEnum::Wait_End, std::make_shared<Task::Wait>(m_param.endWaitParamPtr));

		//ChangeStageStart
		auto fadeEndFunction = [&]() {
			SoundManager::GetInstance()->Stop(SoundManager::ID::GameBGM);                                     //サウンドのストップ。
			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");  //ステージの切り替え。
		};
		m_taskList->DefineTask(TaskEnum::ChangeStageStart,
			[&, fadeEndFunction]() { ScreenFadeManager::GetInstance()->FadeStart(FadeType::Out, fadeEndFunction); },
			[]() { return true; },
			[]() {  }
		);
	}

	void ClearDirection::SelectTask() {
		TaskEnum tasks[] = {
			TaskEnum::Wait_BeforeInsertUI,
			TaskEnum::FadeInBackSprite,
			TaskEnum::InsertUI,
			TaskEnum::Wait_End,
			TaskEnum::ChangeStageStart,
		};

		for (const auto& task : tasks) {
			m_taskList->AddTask(task);
		}
	}

	void ClearDirection::CreateClearUI() {
		auto ui = GetStage()->AddGameObject<ClearUIObject>();

		m_clearUI = ui;
	}

	//--------------------------------------------------------------------------------------
	///	タスクの実装
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------

		InsertUI_Parametor::InsertUI_Parametor():
			scalePopParamPtr(new ScalePop_Parametor(Vec3(1.0f), Vec3(1.0f), 2.5f))
		{}

		InsertUI_Parametor::InsertUI_Parametor(const std::shared_ptr<ScalePop_Parametor>& scalePopParamPtr):
			scalePopParamPtr(scalePopParamPtr)
		{}

		//--------------------------------------------------------------------------------------
		///	UIの挿入本体
		//--------------------------------------------------------------------------------------

		InsertUI::InsertUI(const std::shared_ptr<SpriteObject> owner, const std::shared_ptr<Parametor>& paramPtr) :
			TaskNodeBase(owner), m_paramPtr(paramPtr)
		{}

		void InsertUI::OnStart() {
			auto scale = GetOwner()->GetComponent<Transform>()->GetScale();
			m_paramPtr->scalePopParamPtr->maxScale = scale;
			m_paramPtr->scalePopParamPtr->returnScale = scale;
			m_paramPtr->scalePopParamPtr->deltaType = maru::DeltaType::Real;

			GetOwner()->SetDrawActive(true);
			auto scalePop = GetOwner()->AddComponent<ScalePop>(*m_paramPtr->scalePopParamPtr.get());
			scalePop->StartPop(m_paramPtr->scalePopParamPtr->speed);
		}

		bool InsertUI::OnUpdate() {
			return true;
			auto scalePop = GetOwner()->GetComponent<ScalePop>(false);
			return scalePop->IsPopEnd();
		}

		void InsertUI::OnExit() {

		}

	}

}