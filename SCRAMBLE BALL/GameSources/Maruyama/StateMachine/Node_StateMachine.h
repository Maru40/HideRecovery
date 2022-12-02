/*!
@file Node_StateMachine.h
@brief Node_StateMachineなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "NodeBase.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// コンポーネントの変更関係の情報をまとめた構造体
		//--------------------------------------------------------------------------------------
		struct ChangeComponentParametor
		{
			std::shared_ptr<Component> component; //コンポーネントのポインタ
			bool isStart;                         //開始時にどっちにするか？
			bool isExit;                          //終了時にどっちにするか？

			ChangeComponentParametor(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit
			) :
				component(component),
				isStart(isStart),
				isExit(isExit)
			{}
		};

		//--------------------------------------------------------------------------------------
		/// ステートマシンのインターフェース
		//--------------------------------------------------------------------------------------
		class I_StateNode
		{
		public:
			virtual void OnStart() = 0;
			virtual bool OnUpdate() = 0;
			virtual void OnExit() = 0;

			virtual ~I_StateNode() = default;
		};

		//--------------------------------------------------------------------------------------
		/// ステートマシン用のノード基底クラス
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class NodeBase_StateMachine : public NodeBase, public I_StateNode
		{
			std::weak_ptr<OwnerType> m_owner;	//オーナー

		public:
			NodeBase_StateMachine(const std::shared_ptr<OwnerType>& owner) :
				NodeBase_StateMachine(0, owner)
			{}

			NodeBase_StateMachine(const int index, const std::shared_ptr<OwnerType>& owner):
				NodeBase(index),
				m_owner(owner)
			{}

			virtual ~NodeBase_StateMachine() = default;

		public:

			/// <summary>
			/// 所有者の取得
			/// </summary>
			/// <returns>所有者</returns>
			std::shared_ptr<OwnerType> GetOwner() const {
				auto owner = m_owner.lock();
				return owner ? owner : nullptr;
			}

		};

		//--------------------------------------------------------------------------------------
		/// ステートマシン用のノード基底クラスの拡張(コンポーネントの切り替えを追加)
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class NodeBase_StateMachine_Ex : public NodeBase_StateMachine<OwnerType>
		{
			std::vector<ChangeComponentParametor> m_changeParams;	//コンポーネント変更用のパラメータ配列

		public:
			NodeBase_StateMachine_Ex(const std::shared_ptr<OwnerType>& owner):
				NodeBase_StateMachine_Ex(0, owner)
			{}

			NodeBase_StateMachine_Ex(const int index, const std::shared_ptr<OwnerType>& owner):
				NodeBase_StateMachine<OwnerType>(index, owner)
			{}

			virtual ~NodeBase_StateMachine_Ex() = default;

		protected:

			/// <summary>
			/// 開始と終了時に切り替えるコンポーネントの追加
			/// </summary>
			/// <param name="component">切り替えるコンポーネントのポインタ</param>
			/// <param name="isStart">スタート時にどっちに切り替える</param>
			/// <param name="isExit">終了時にどっちに切り替えるか</param>
			void AddChangeComponent(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit)
			{
				if (component == nullptr) {  //nullptrなら追加しない
					return;
				}

				ChangeComponentParametor param(component, isStart, isExit);
				m_changeParams.push_back(param);
			}

			/// <summary>
			/// 開始と終了時に切り替えるコンポーネントの追加
			/// </summary>
			/// <param name="param">切り替える情報をまとめたパラメータ</param>
			void AddChangeComponent(const ChangeComponentParametor& param) {
				if (param.component == nullptr) {
					return;
				}
				m_changeParams.push_back(param);
			}

			/// <summary>
			/// 登録されたコンポーネントの切り替えを行う(スタート時)
			/// </summary>
			void StartChangeComponents() {
				for (auto& param : m_changeParams) {
					param.component->SetUpdateActive(param.isStart);
				}
			}

			/// <summary>
			/// 登録されたコンポーネントの切り替えを行う(終了時)
			/// </summary>
			void ExitChangeComponents() {
				for (auto& param : m_changeParams) {
					param.component->SetUpdateActive(param.isExit);
				}
			}

		public:
			virtual void OnStart() override {
				StartChangeComponents();
			}

			virtual void OnExit() override {
				ExitChangeComponents();
			}

		};

		namespace StateNode {

			template<class OwnerType>
			class EmptyNode : public NodeBase_StateMachine<OwnerType>
			{
			public:
				EmptyNode(const int index, const std::shared_ptr<OwnerType>& owner) :
					NodeBase_StateMachine(index, owner)
				{}

				virtual ~EmptyNode() = default;

				void OnStart() override {}

				bool OnUpdate() override { return true; }

				void OnExit() override {}
			};

		}

	}
}