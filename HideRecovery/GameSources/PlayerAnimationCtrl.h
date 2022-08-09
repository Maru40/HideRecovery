
/*!
@file PlayerAnimationCtrl.h
@brief PlayerAnimationCtrl.hクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "AnimationCtrl.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// アニメーションのステート
	//--------------------------------------------------------------------------------------
	enum class PlayerAnimationCtrl_State {
		Idle,				//待機
		Walk,				//歩く
		PutItem_Floor,		//床に置く
		PutItem_HideObject,	//隠すオブジェクトに置く
	};

	//--------------------------------------------------------------------------------------
	/// 遷移メンバー
	//--------------------------------------------------------------------------------------
	struct PlayerAnimationCtrl_TransitionMember {
		float walkSpeed;

		PlayerAnimationCtrl_TransitionMember();
	};

	//--------------------------------------------------------------------------------------
	/// アニメーション追加パラメータ
	//--------------------------------------------------------------------------------------
	struct AnimationParametor {
		using State = PlayerAnimationCtrl_State;

		State stateType;
		wstring stateName;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;
		float updateSpeed;		//更新スピード

		std::function<void()> startEvent;
		std::function<bool(const float)> updateEvent;
		std::function<void()> exitEvent;

		AnimationParametor();

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float updateSpeed,
			const std::shared_ptr<I_AnimationStateNode>& stateNode
		);

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float updateSpeed,
			const function<void()>& startEvent,
			const function<bool(const float)>& updateEvent,
			const function<void()>& exitEvent
		);

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const float updateSpeed,
			const function<void()>& startEvent,
			const function<bool(const float)>& updateEvent,
			const function<void()>& exitEvent
		);
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーのアニメーション管理
	//--------------------------------------------------------------------------------------
	class PlayerAnimationCtrl : public AnimationCtrl
	{
	public:
		using State = PlayerAnimationCtrl_State;
		using TransitionMember = PlayerAnimationCtrl_TransitionMember;

	private:
		unordered_map<State, AnimationParametor> m_parametorMap;	//パラメータマップ

		State m_currentState;						//現在のステート
		TransitionMember m_transitionMember;		//遷移条件メンバー

		std::weak_ptr<SmBaseDraw> m_drawComponent;	//描画コンポ―ネント

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnStart() override;
		void OnUpdate() override;

	private:
		//--------------------------------------------------------------------------------------
		/// アニメーションイベント
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 待機
		/// </summary>
		bool Idle(const float speed);

		/// <summary>
		/// 歩く
		/// </summary>
		bool Walk(const float speed);

		/// <summary>
		/// デフォルト再生関数
		/// </summary>
		void DefaultPlay(const bool isEndTransitionIdle = true);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// アニメーションの切り替え
		/// </summary>
		void ChangeAnimation(const State& state);

		/// <summary>
		/// 強制アニメーション切り替え
		/// </summary>
		/// <param name="state">切り替えるタイプ</param>
		void ChangeForceAnimation(const State& state);

		/// <summary>
		/// 現在のアニメーションステートを取得
		/// </summary>
		State GetCurrentAnimaiton() const noexcept { return m_currentState; }

		/// <summary>
		/// 現在のアニメーション更新速度を取得
		/// </summary>
		float GetCurrentUpdateSpeed() const { return m_parametorMap.at(m_currentState).updateSpeed; }
	};


	//--------------------------------------------------------------------------------------
	/// アニメーションステートノード
	//--------------------------------------------------------------------------------------

	namespace AnimationStateNode {

		//--------------------------------------------------------------------------------------
		/// 床にアイテムを置くアニメーション
		//--------------------------------------------------------------------------------------
		class PutItem_Floor : public AnimationStateNodeBase
		{
		public:
			PutItem_Floor(const std::shared_ptr<GameObject>& objPtr);

			virtual ~PutItem_Floor() = default;

			void OnStart() override;
			bool OnUpdate(const float speed) override;
			void OnExit() override;
		};

		//--------------------------------------------------------------------------------------
		/// 隠すオブジェクトにアイテムを置くアニメーション
		//--------------------------------------------------------------------------------------
		class PutItem_HideObject : public AnimationStateNodeBase
		{
		public:
			PutItem_HideObject(const std::shared_ptr<GameObject>& objPtr);

			virtual ~PutItem_HideObject() = default;

			void OnStart() override;
			bool OnUpdate(const float speed) override;
			void OnExit() override;
		};
	}

}

//endbasecross