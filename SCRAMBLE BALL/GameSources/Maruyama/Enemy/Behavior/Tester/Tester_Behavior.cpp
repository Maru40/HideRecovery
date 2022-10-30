/*!
@file Tester_Behavior.cpp
@brief Tester_Behavior‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Tester_Behavior.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Utility.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		 namespace Behavior {

			TesterTree::TesterTree(const std::shared_ptr<GameObject>& objPtr) :
				Component( objPtr), 
				m_behaviorTree(new BehaviorTree<TesterTree_Type>())
			{}

			void TesterTree::OnCreate() {
				CreateNode();
				CreateEdge();
			}

			void TesterTree::OnUpdate() {
				m_behaviorTree->OnUpdate();
			}

			void TesterTree::CreateNode() {
				m_behaviorTree->AddSelecter(BehaviorType::First, std::make_shared<Selecter>());

				m_behaviorTree->AddSelecter(BehaviorType::Selecter2, std::make_shared<Selecter>());

				m_behaviorTree->AddTask(BehaviorType::Task1, std::make_shared<Task::TesterTaskFirst>());

				m_behaviorTree->AddTask(BehaviorType::Task2, std::make_shared<Task::TesterTaskSecond>());

				m_behaviorTree->AddTask(BehaviorType::Task3, std::make_shared<Task::TesterTaskThird>());
			}

			void TesterTree::CreateEdge() {
				m_behaviorTree->AddEdge(BehaviorType::First, BehaviorType::Selecter2, std::make_shared<PriorityControllerBase>(2.0f));
				m_behaviorTree->AddEdge(BehaviorType::First, BehaviorType::Task1, std::make_shared<PriorityControllerBase>(1.0f));

				m_behaviorTree->AddEdge(BehaviorType::Selecter2, BehaviorType::Task2, std::make_shared<PriorityControllerBase>(2.0f));
				m_behaviorTree->AddEdge(BehaviorType::Selecter2, BehaviorType::Task3, std::make_shared<PriorityControllerBase>(1.0f));
			}


			namespace Task {
				TesterTaskFirst::TesterTaskFirst() : 
					m_timer(new GameTimer(0.0f)) 
				{}
				void TesterTaskFirst::OnStart() { 
					m_timer->ResetTimer(3.0f); 
					Debug::GetInstance()->Log(L"Start::First");
				};
				bool TesterTaskFirst::OnUpdate() { 
					m_timer->UpdateTimer();
					return m_timer->IsTimeUp();
				};
				void TesterTaskFirst::OnExit() { Debug::GetInstance()->Log(L"Exit::First"); };


				TesterTaskSecond::TesterTaskSecond() : 
					m_timer(new GameTimer(0.0f)) 
				{}
				void TesterTaskSecond::OnStart() { 
					m_timer->ResetTimer(3.0f); 
					Debug::GetInstance()->Log(L"Start::Second");
				};
				bool TesterTaskSecond::OnUpdate() { 
					m_timer->UpdateTimer();
					return m_timer->IsTimeUp();
				};
				void TesterTaskSecond::OnExit() { Debug::GetInstance()->Log(L"Exit::Second"); };


				TesterTaskThird::TesterTaskThird() :
					m_timer(new GameTimer(0.0f)) 
				{}
				void TesterTaskThird::OnStart() { 
					m_timer->ResetTimer(3.0f); 
					Debug::GetInstance()->Log(L"Start::Third");
				};
				bool TesterTaskThird::OnUpdate() {
					m_timer->UpdateTimer();
					return m_timer->IsTimeUp();
				};
				void TesterTaskThird::OnExit() { Debug::GetInstance()->Log(L"Exit::Third"); };
			}
			   
		}

	}
}