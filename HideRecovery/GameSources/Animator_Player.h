/*!
@file Animator_Player.h
@brief Animator_Playerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "AnimationHelper.h"

namespace basecross
{

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class StructMember, class StateEnum>
	class AnimatorMainStateMachine;

	template<class StructMember, class StateEnum>
	class Animator;

	template<class TaskEnum>
	class TaskList;

	struct Task_MovePositions_Parametor;

	//--------------------------------------------------------------------------------------
	/// プレイヤーアニメーションの遷移条件メンバー
	//--------------------------------------------------------------------------------------
	struct PlayerAnimator_TransitionMember
	{
		float walkSpeed = 0.0f;           //移動スピード
		float transitionWalkSpeed = 3.0f; //移動に遷移するスピード
		Trigger tackleTrigger;            
		Trigger aimTrigger;
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーアニメーションのステートタイプ
	//--------------------------------------------------------------------------------------
	enum class PlayerAnimator_StateEnum {
		Idle,       //待機
		WalkStart,  //歩行開始
		Walk,       //歩行
		WalkEnd,    //歩行終了
		Tackle,     //タックル
		TackleEnd,  //タックする終了
		Aim,        //エイム
		Shot,       //撃つ処理
		Damage,     //ダメージ
		Death,      //死亡
		WinIdle,    //勝利待機
		WinMove,    //勝利移動
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーのアニメーション
	//--------------------------------------------------------------------------------------
	class PlayerAnimator : public basecross::Animator<PlayerAnimator_TransitionMember, PlayerAnimator_StateEnum>
	{
	public:
		using TransitionMember = PlayerAnimator_TransitionMember;
		using StateEnum = PlayerAnimator_StateEnum;
		using StateMachine = basecross::AnimatorMainStateMachine<TransitionMember, StateEnum>;

	private:
		ex_weak_ptr<StateMachine> m_stateMachine;                         //ステートマシン

		std::shared_ptr<Task_MovePositions_Parametor> m_winMoveParamPtr;  //勝利移動パラメータ
		 
		bool m_isPlayPlayerWalkSound;                                     //歩行サウンドを利用するかどうか

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PlayerAnimator(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ステートの変更
		/// </summary>
		/// <param name="stateEnum">ステートタイプ</param>
		virtual void ChangeState(const StateEnum& stateEnum) override;

		/// <summary>
		/// ステートの強制変更
		/// </summary>
		/// <param name="stateEnum">ステートタイプ</param>
		void ForceChangeState(const StateEnum& stateEnum);

	private:
		/// <summary>
		/// ノードの生成
		/// </summary>
		void CreateNode();

		/// <summary>
		/// 待機ノードの生成
		/// </summary>
		void CreateIdleNode();

		/// <summary>
		/// 歩行開始ノードの生成
		/// </summary>
		void CreateWalkStartNode();

		/// <summary>
		/// 歩行ノードの生成
		/// </summary>
		void CreateWalkNode();

		/// <summary>
		/// 歩行終了ノードの生成
		/// </summary>
		void CreateWalkEndNode();

		/// <summary>
		/// タックルノードの生成
		/// </summary>
		void CreateTackleNode();

		/// <summary>
		/// タックル終了ノードの生成
		/// </summary>
		void CreateTacleEndNode();

		/// <summary>
		/// エイムノードの生成
		/// </summary>
		void CreateAimNode();

		/// <summary>
		/// ショットノードの生成
		/// </summary>
		void CreateShotNode();

		/// <summary>
		/// ダメージノードの生成
		/// </summary>
		void CreateDamageNode();

		/// <summary>
		/// 死亡ノードの生成
		/// </summary>
		void CreateDeathNode();

		/// <summary>
		/// 勝利待機ノードの生成
		/// </summary>
		void CreateWinIdleNode();

		/// <summary>
		/// 勝利移動の生成
		/// </summary>
		void CreateWinMoveNode();

		/// <summary>
		/// 死亡アニメーション終了イベント
		/// </summary>
		void EndDeathAnimationEvent();

	public:

	};

}