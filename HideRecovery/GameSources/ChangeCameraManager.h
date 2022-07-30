/*!
@file ChangeCameraManager.h
@brief ChangeCameraManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "I_BrackBoard.h"
#include "SingletonComponent.h"

namespace basecross {

	//前方宣言
	class GameTimer;

	template<class NodeType, class EnumType, class TransitionMember>
	class EnemyMainStateMachine;

	//--------------------------------------------------------------------------------------
	///	カメラ切り替え管理のブラックボードパラメータ
	//--------------------------------------------------------------------------------------
	struct ChangeCameraManager_BlackBoardParametor
	{
		ex_weak_ptr<GameObject> lookAtTarget;   //注視するターゲット
	};

	//--------------------------------------------------------------------------------------
	///	カメラ切り替え管理クラス
	//--------------------------------------------------------------------------------------
	class ChangeCameraManager : public maru::SingletonComponent<ChangeCameraManager>, public I_BrackBoard<ChangeCameraManager_BlackBoardParametor>
	{
	public:
		/// <summary>
		/// ステートマシンのステートタイプ
		/// </summary>
		enum class State {
			TPS,    //三人称視点
			Look,   //注視
			Tackle, //タックル
			Debug,  //デバッグ
		};

		/// <summary>
		/// ステートマシンの遷移用パラメータ
		/// </summary>
		struct TransitionMember {

		};

		using StateMachine = EnemyMainStateMachine<GameObject, State, TransitionMember>;

	private:
		std::unique_ptr<GameTimer> m_tackleTimer;      //タックル経過時間計測用クラス
		std::unique_ptr<StateMachine> m_stateMachine;  //ステートマシンクラス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ChangeCameraManager(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate2() override;

	private:
		/// <summary>
		/// ノードの生成
		/// </summary>
		void CreateNode();

		/// <summary>
		/// エッジの生成
		/// </summary>
		void CreateEdge();

	public:
		/// <summary>
		/// ステートの変更
		/// </summary>
		/// <param name="state">変更するステート</param>
		void ChangeState(const State state);

		/// <summary>
		/// 現在のステートを取得
		/// </summary>
		State GetNowState() const noexcept;
	};

}

//endbasecross