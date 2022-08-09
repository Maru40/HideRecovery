
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
		Wait,
		Walk,
		Put_Floor,
		Put_HideObject,
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
	/// プレイヤーのアニメーション管理
	//--------------------------------------------------------------------------------------
	class PlayerAnimationCtrl : public AnimationCtrl
	{
	public:
		using State = PlayerAnimationCtrl_State;

	private:
		unordered_map<State, wstring> m_stateStringMap;
		unordered_map<wstring, std::function<void(PlayerAnimationCtrl&)>> m_animations;

		State m_currentState;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		//--------------------------------------------------------------------------------------
		/// 再生したいアニメーション
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 待機
		/// </summary>
		void Wait();

		/// <summary>
		/// 歩く
		/// </summary>
		void Walk();

		/// <summary>
		/// デフォルト再生関数
		/// </summary>
		void DefaultPlay(const float speed = 15.0f);

	public:
		/// <summary>
		/// アニメーションの設定
		/// </summary>
		/// <returns>アニメーション</returns>
		void SetAnimaiton(const wstring& animeName, const function<void(PlayerAnimationCtrl&)> func);

		/// <summary>
		/// アニメーションの切り替え
		/// </summary>
		void ChangeAnimation(const wstring& animeType);

		/// <summary>
		/// 現在のアニメーションステートを取得
		/// </summary>
		wstring GetCurrentAnimaiton() const {
			auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
			return draw->GetCurrentAnimation();
		}
	};

}

//endbasecross