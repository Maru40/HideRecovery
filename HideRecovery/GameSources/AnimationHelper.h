#pragma once
#include<unordered_map>
#include"DelegateSystem.h"

namespace basecross
{
	/// <summary>
	/// ステートの抽象クラス
	/// </summary>
	/// <typeparam name="StructMember">使用する構造体</typeparam>
	/// <typeparam name="StateEnum">使用する列挙体</typeparam>
	template<class StructMember, class StateEnum>
	class StateBase
	{
	public:
		/// <summary>
		/// 遷移条件型
		/// </summary>
		using ConditionFunction = bool(*)(const StructMember&);

		/// <summary>
		/// 遷移情報構造体
		/// </summary>
		struct TransitionCondition
		{
			/// <summary>
			/// 遷移する条件
			/// </summary>
			ConditionFunction conditionFunction;
			/// <summary>
			/// 遷移先のステート
			/// </summary>
			StateEnum nextStateEnum;
			/// <summary>
			/// アニメーション終了時に条件判定するか
			/// </summary>
			bool isExitEnd;
		};

	private:

		/// <summary>
		/// 遷移情報配列
		/// </summary>
		std::vector<TransitionCondition> m_transitionConditions;

		/// <summary>
		/// 自分自身のステート
		/// </summary>
		StateEnum m_state;

		/// <summary>
		/// アニメーションループが終了しているか
		/// </summary>
		bool m_isEndLoop = false;

	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="state">自分自身のステート</param>
		StateBase(const StateEnum state) :
			m_state(state)
		{

		}

		/// <summary>
		/// 遷移を追加する
		/// </summary>
		/// <param name="function">遷移する条件</param>
		/// <param name="nextState">遷移先のステート</param>
		/// <param name="isExitEnd">アニメーションが終了した際に条件判定するか</param>
		void AddTransition(ConditionFunction function, const StateEnum nextState,const bool isExitEnd)
		{
			TransitionCondition transition{};
			transition.conditionFunction = function;
			transition.nextStateEnum = nextState;
			transition.isExitEnd = isExitEnd;

			m_transitionConditions.push_back(transition);
		}

		/// <summary>
		/// 遷移条件を検証して次に再生されるステートを取得する
		/// </summary>
		/// <param name="structMember">遷移条件に使う構造体</param>
		/// <returns>次に再生されるステート</returns>
		StateEnum CheckTransition(const StructMember& structMember)
		{
			for (auto& transitionCondition : m_transitionConditions)
			{
				if (transitionCondition.isExitEnd && !m_isEndLoop)
				{
						continue;
				}

				if (transitionCondition.conditionFunction(structMember))
				{
					m_isEndLoop = false;

					return transitionCondition.nextStateEnum;
				}
			}

			m_isEndLoop = false;

			return m_state;
		}

		/// <summary>
		/// ステートに突入時に呼ばれる関数
		/// </summary>
		virtual void Entry() {}

		/// <summary>
		/// 毎フレーム呼ばれる関数
		/// </summary>
		/// <param name="member">ステートデータ構造体</param>
		virtual void Play(const StructMember& member) = 0;

		/// <summary>
		/// ステートからの脱出時に呼ばれる関数
		/// </summary>
		virtual void Exit() {}

		/// <summary>
		/// 現在のステートを返す
		/// </summary>
		/// <returns>現在のステート</returns>
		virtual StateEnum GetNowState() const = 0;

		/// <summary>
		/// 自分自身のステートを返す関数
		/// </summary>
		/// <returns>自分のステート</returns>
		StateEnum GetThisState() const
		{
			return m_state;
		}

		virtual float GetStateRateOfTime() const = 0;

		/// <summary>
		/// アニメーションループが終了しているかを設定する
		/// </summary>
		/// <param name="isEndLoop">アニメーションループが終了しているか</param>
		void SetIsEndLoop(const bool isEndLoop)
		{
			m_isEndLoop = isEndLoop;
		}
	};

	/// <summary>
	/// アニメーションに必要なデータ
	/// </summary>
	struct AnimationClip
	{
		/// <summary>
		/// アニメーションのリソースキー
		/// </summary>
		std::wstring animationKey;
		/// <summary>
		/// 再生時間
		/// </summary>
		int playTime;
		/// <summary>
		/// ループするかどうか
		/// </summary>
		bool isLoop = false;
	};

	/// <summary>
	/// アニメーションステート抽象クラス
	/// </summary>
	template<class StructMember, class StateEnum>
	class AnimationStateBase : public StateBase<StructMember, StateEnum>
	{
		/// <summary>
		/// アニメーションイベント構造体
		/// </summary>
		struct AnimationEvent
		{
			/// <summary>
			/// 発行される時間
			/// </summary>
			float actionTime;
			/// <summary>
			/// 呼べれるイベント
			/// </summary>
			itbs::Utility::Delegate<void()> actionEvent;
			/// <summary>
			/// アクションし終わったか
			/// </summary>
			bool isAction;

			AnimationEvent(const float actionTime, const itbs::Utility::Delegate<void()>& actionEvent) :
				actionTime(actionTime),
				actionEvent(actionEvent),
				isAction(false)
			{

			}
		};

		/// <summary>
		/// ひとつ前のアニメーションの時間
		/// </summary>
		float m_beforeCurrentAnimationTime = 0.0f;

		/// <summary>
		/// アニメーションイベント配列
		/// </summary>
		std::vector<AnimationEvent> m_animationEvents;

		/// <summary>
		/// ゲームオブジェクト
		/// </summary>
		std::shared_ptr<GameObject> m_gameobject;

		/// <summary>
		/// アニメーションするボーンモデル
		/// </summary>
		std::shared_ptr<PNTBoneModelDraw> m_modelDraw;

		/// <summary>
		/// アニメーションデータ
		/// </summary>
		AnimationClip m_animationClip;

		/// <summary>
		/// ステート突入時に呼ばれるイベント配列
		/// </summary>
		std::vector<std::function<void()>> m_entryEvents;
		/// <summary>
		/// ステート突入時に呼ばれるイベント配列
		/// </summary>
		std::vector<std::function<void()>> m_exitEvents;

		std::vector<std::function<void()>> m_updateEvents;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stateEnum">自分自身のステート</param>
		/// <param name="modelDraw">アニメーションするボーンモデルDraw</param>
		/// <param name="animationClip">アニメーションデータ</param>
		AnimationStateBase(const StateEnum& stateEnum, const std::shared_ptr<PNTBoneModelDraw>& modelDraw,
			const AnimationClip& animationClip) :
			StateBase(stateEnum),
			m_modelDraw(modelDraw),
			m_animationClip(animationClip),
			m_gameobject(modelDraw->GetGameObject())
		{
		}

		template<class T,
			std::enable_if_t<std::is_base_of_v<Component, T>, std::nullptr_t> = nullptr>
			void AddAnimationEvent(const float actionTime, void (T::* animationEvent)(), std::shared_ptr<T> animator)
		{
			auto eventDelegate = itbs::Utility::Delegate<void()>(animator, animationEvent);
			m_animationEvents.push_back(AnimationEvent(actionTime, eventDelegate));
		}

		void AddUpdateEvent(const std::function<void()>& function) {
			m_updateEvents.push_back(function);
		}

		/// <summary>
		/// ステートに突入時に呼ばれる関数
		/// </summary>
		void Entry() override
		{
			m_modelDraw->SetMultiMeshResource(m_animationClip.animationKey);
			m_modelDraw->AddAnimation(m_animationClip.animationKey, 0, m_animationClip.playTime, m_animationClip.isLoop);
			m_modelDraw->ChangeCurrentAnimation(m_animationClip.animationKey);

			for (auto& entryEvent : m_entryEvents)
			{
				entryEvent();
			}
		}

		/// <summary>
		/// ステート突入時のイベントを追加する
		/// </summary>
		/// <param name="entryEvent">ステート突入時のイベント</param>
		void AddEntryEvent(const std::function<void()>& entryEvent)
		{
			m_entryEvents.push_back(entryEvent);
		}

		/// <summary>
		/// ステート脱出時のイベントを追加する
		/// </summary>
		/// <param name="exitEvent">ステート脱出時のイベント</param>
		void AddExitEvent(const std::function<void()>& exitEvent)
		{
			m_exitEvents.push_back(exitEvent);
		}

		/// <summary>
		/// 毎フレーム呼ばれる関数
		/// </summary>
		void Play(const StructMember& member) override
		{
			float currentTime = m_modelDraw->GetCurrentAnimationTime();
			bool isReset = m_beforeCurrentAnimationTime >= currentTime;

			for (AnimationEvent& animationEvent : m_animationEvents)
			{
				if (isReset)
				{
					animationEvent.isAction = false;
				}

				if (animationEvent.isAction)
				{
					continue;
				}

				if (currentTime >= animationEvent.actionTime)
				{
					animationEvent.actionEvent();
					animationEvent.isAction = true;
				}
			}

			if (m_modelDraw->IsTargetAnimeEnd())
			{
				SetIsEndLoop(true);

				return;
			}

			for (auto& function : m_updateEvents) {
				if (function) {
					function();
				}
			}

			//for (AnimationEvent& animationEvent : m_animationEvents)
			//{
			//	if (isReset)
			//	{
			//		animationEvent.isAction = false;
			//	}

			//	if (animationEvent.isAction)
			//	{
			//		continue;
			//	}

			//	if (currentTime >= animationEvent.actionTime)
			//	{
			//		animationEvent.actionEvent();
			//		animationEvent.isAction = true;
			//	}
			//}

			m_beforeCurrentAnimationTime = currentTime;
		}

		/// <summary>
		/// ステート脱出時に呼ばれる関数
		/// </summary>
		void Exit() override
		{
			for (auto& exitEvent : m_exitEvents)
			{
				exitEvent();
			}
		}

		/// <summary>
		/// 現在のステートを取得する
		/// </summary>
		/// <returns>現在のステート</returns>
		StateEnum GetNowState() const override
		{
			return GetThisState();
		}

		float GetStateRateOfTime() const override
		{
			auto currentTime = m_modelDraw->GetCurrentAnimationTime();
			return currentTime / (static_cast<float>(m_animationClip.playTime) / 30.0f);
		}
	};

	/// <summary>
	/// アニメーション用ステートマシーンクラス
	/// </summary>
	template<class StructMember, class StateEnum>
	class AnimatorMainStateMachine
	{
		/// <summary>
		/// ステート型
		/// </summary>
		using StateBasePtr = std::shared_ptr<StateBase<StructMember, StateEnum>>;

		/// <summary>
		/// ステート列挙体とステートをマップしたもの
		/// </summary>
		std::unordered_map<StateEnum, StateBasePtr> m_states;

		/// <summary>
		/// モデルのポインタ
		/// </summary>
		std::shared_ptr<PNTBoneModelDraw> m_modelDraw;

		/// <summary>
		/// 現在のステート
		/// </summary>
		StateEnum m_nowStateEnum;

		/// <summary>
		/// ステートが一つも入っていないか
		/// </summary>
		bool isEmpty = true;

		/// <summary>
		/// ステートマシンの再生の最初の一回か
		/// </summary>
		bool isFirst = true;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="modelDraw">モデル管理のポインタ</param>
		AnimatorMainStateMachine(const std::shared_ptr<PNTBoneModelDraw>& modelDraw) :
			m_modelDraw(modelDraw)
		{

		}

		/// <summary>
		/// アニメーションステートを作成する関数
		/// </summary>
		/// <param name="stateEnum">ステートキー</param>
		/// <param name="animationName">アニメーションのリソースキー</param>
		/// <param name="animationLength">アニメーションの再生秒数</param>
		/// <param name="isLoop">ループするかどうか</param>
		/// <returns>アニメーションステート</returns>
		std::shared_ptr<AnimationStateBase<StructMember, StateEnum>> CreateAnimationState(const StateEnum stateEnum,
			const std::wstring& animationName, const int animationLength, const bool isLoop)
		{
			AnimationClip animationClip;
			animationClip.animationKey = animationName;
			animationClip.isLoop = isLoop;
			animationClip.playTime = animationLength;

			auto state = std::make_shared<AnimationStateBase<StructMember, StateEnum>>(stateEnum, m_modelDraw, animationClip);
			m_states.insert(std::make_pair(stateEnum, state));

			if (isEmpty)
			{

				isEmpty = false;
				m_nowStateEnum = stateEnum;
			}

			return state;
		}

		/// <summary>
		/// 再生する際に呼ばれる
		/// </summary>
		/// <param name="member">ステートマシンが使用するデータ構造体</param>
		/// <param name="animationSpeed">アニメーションスピード</param>
		void Play(const StructMember& member,const float animationSpeed)
		{
			if (isEmpty)
			{
				return;
			}

			if (isFirst)
			{
				isFirst = false;
				m_states.at(m_nowStateEnum)->Entry();
			}

			m_modelDraw->UpdateAnimation(App::GetApp()->GetElapsedTime() * animationSpeed);

			auto state = m_states.at(m_nowStateEnum);

			auto nextState = state->CheckTransition(member);

			while (nextState != state->GetThisState())
			{
				//ChangeState(nextState, member);
				state->Exit();

				m_nowStateEnum = nextState;

				state = m_states[m_nowStateEnum];

				state->Entry();

				nextState = state->CheckTransition(member);
			}

			state->Play(member);
		}

		void ChangeState(const StateEnum& nextState, const StructMember& member) {
			auto state = m_states[m_nowStateEnum];

			state->Exit();

			m_nowStateEnum = nextState;

			state = m_states[m_nowStateEnum];

			state->Entry();

			state->CheckTransition(member);
		}

		/// <summary>
		/// 現在のステートを取得する
		/// </summary>
		/// <returns>現在のステート</returns>
		StateEnum GetNowState() const
		{
			return m_nowStateEnum;
		}

		float GetStateRateOfTime() const
		{
			return m_states.at(m_nowStateEnum)->GetStateRateOfTime();
		}
	};

	/// <summary>
	/// アニメーション管理コンポーネント
	/// </summary>
	template<class StructMember, class StateEnum>
	class Animator : public Component
	{
		/// <summary>
		/// ステートマシーン型
		/// </summary>
		using StateMachineType = AnimatorMainStateMachine<StructMember, StateEnum>;
		/// <summary>
		/// ステートマシンで使用する構造体
		/// </summary>
		StructMember m_structMember;

		/// <summary>
		/// ステートマシンのポインタ
		/// </summary>
		std::shared_ptr<StateMachineType> m_animatorStateMachine;
		/// <summary>
		/// アニメーションに使うモデルの描画ポインタ
		/// </summary>
		std::shared_ptr<PNTBoneModelDraw> m_boneModelDraw;

		float m_animationSpeed = 1.0f;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		Animator(const std::shared_ptr<GameObject>& owner) :
			Component(owner)
		{
		}

		/// <summary>
		/// アニメーションステートマシーンを作成する関数
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<StateMachineType> CreateAnimationStateMchine()
		{
			if (!m_boneModelDraw)
			{
				m_boneModelDraw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
			}

			m_animatorStateMachine = std::make_shared<StateMachineType>(m_boneModelDraw);

			return m_animatorStateMachine;
		}

		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ステートマシーンで使用する構造体の参照を取得する
		/// </summary>
		/// <returns>ステートマシーンで使用する構造体の参照</returns>
		StructMember& GetMemberRefarence()
		{
			return m_structMember;
		}

		/// <summary>
		/// 現在のステートを取得する
		/// </summary>
		/// <returns>現在のステート</returns>
		StateEnum GetNowState() const
		{
			return m_animatorStateMachine->GetNowState();
		}

		float GetStateRateOfTime() const
		{
			return m_animatorStateMachine->GetStateRateOfTime();
		}

		void OnUpdate() override
		{
			if (!m_animatorStateMachine)
			{
				return;
			}

			m_animatorStateMachine->Play(m_structMember, m_animationSpeed);
		}

		virtual void ChangeState(const StateEnum& stateEnum) {
			m_animatorStateMachine->ChangeState(stateEnum, m_structMember);
		}
	};

}