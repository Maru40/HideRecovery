
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
		Idle,
		Walk,
		Put_Floor,
		Put_HideObject,
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
	template<class T>
	struct AddAnimeParam {
		using State = PlayerAnimationCtrl_State;

		State stateType;
		wstring stateName;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;
		float updateSpeed;		//更新スピード

		function<void(T&)> func;

		AddAnimeParam(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const function<void(T&)> func
		) :
			AddAnimeParam(state, name, startTime, endTime, loop, 1.0f, func)
		{}

		AddAnimeParam(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const function<void(T&)> func
		) :
			stateType(state),
			stateName(name),
			startTime(startTime),
			loop(loop),
			timeParSecond(parSecond),
			func(func)
		{
			timeLength = endTime - startTime;
		}
	};

	//--------------------------------------------------------------------------------------
	/// アニメーションパラメータ
	//--------------------------------------------------------------------------------------
	struct AnimationParametor {
		wstring stateName;	//ステート登録に使用した名前
		float updateSpeed;	//更新スピード
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
		unordered_map<State, wstring> m_stateStringMap;									//Stateをwstringで登録するマップ。
		unordered_map<State, std::function<void(PlayerAnimationCtrl&)>> m_animations;	//アニメーションの更新イベントマップ

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
		/// 再生したいアニメーション
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 待機
		/// </summary>
		void Idle();

		/// <summary>
		/// 歩く
		/// </summary>
		void Walk();

		/// <summary>
		/// デフォルト再生関数
		/// </summary>
		void DefaultPlay(const float speed = 15.0f);

		/// <summary>
		/// アニメーションの設定
		/// </summary>
		/// <returns>アニメーション</returns>
		void SetAnimaiton(const State& state, const function<void(PlayerAnimationCtrl&)> func);

	public:
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
		State GetCurrentAnimaiton() const noexcept { m_currentState; }
	};

}

//endbasecross