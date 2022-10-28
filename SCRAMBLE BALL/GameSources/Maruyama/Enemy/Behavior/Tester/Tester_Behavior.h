/*!
@file Tester_Behavior.h
@brief Tester_BehaviorÇ»Ç«
íSìñé“ÅFä€éR óTäÏ
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Node.h" 
#include "../Interface/I_Selecter.h"
#include "../Interface/I_Task.h"

namespace basecross {

	class GameTimer;

	namespace maru {

		namespace Behavior {

			template<class EnumType>
			class BehaviorTree;

			enum class TesterTree_Type {
				First,
				Selecter2,
				Task1,
				Task2,
				Task3
			};

			class TesterTree : public Component 
			{
			public:
				using BehaviorType = TesterTree_Type;

			private:
				std::unique_ptr<BehaviorTree<TesterTree_Type>> m_behaviorTree;

			public:
				TesterTree(const std::shared_ptr<GameObject>& objPtr);

				void OnCreate() override;
				void OnUpdate() override;

			private:

				void CreateNode();
				void CreateEdge();
			};


			//namespace Selecter {

			//	class TesterFirst : public SelecterBase
			//	{

			//	public:


			//	};


			//	class TesterSelecterSecond : public SelecterBase
			//	{

			//	public:


			//	};

			//}

			namespace Task 
			{
				class TesterTaskFirst : public Behavior::I_Task
				{
					std::unique_ptr<GameTimer> m_timer;

				public:
					TesterTaskFirst();

					void OnStart() override ;
					bool OnUpdate() override ;
					void OnExit() override ;
				};

				class TesterTaskSecond : public I_Task
				{
					std::unique_ptr<GameTimer> m_timer;

				public:
					TesterTaskSecond();

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;
				};

				class TesterTaskThird : public I_Task
				{
					std::unique_ptr<GameTimer> m_timer;

				public:
					TesterTaskThird();

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;
				};
			}

		}
	}
}