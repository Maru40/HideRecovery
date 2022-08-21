
/*!
@file GoalAnimationController.h
@brief GoalAnimationControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	namespace Task {
		struct Wait_Parametor;
	}

	//--------------------------------------------------------------------------------------
	/// ゴール中のアニメーションコントローラーのパラメータ
	//--------------------------------------------------------------------------------------
	struct GoalAnimationController_Parametor
	{
		Vec3 startPosition;		//開始位置
		Vec3 dunkPosition;		//ダンクする位置
		Vec3 jumpPower;			//ジャンプする力
		float dunkMoveSpeed;	//ダンクにかける時間

		std::shared_ptr<Task::Wait_Parametor> preliminaryJumpParam;	//ジャンプ溜めタスクパラメータ
		//ダンクするところまでの移動

		GoalAnimationController_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// ゴール中のアニメーションコントローラー
	//--------------------------------------------------------------------------------------
	class GoalAnimationController : public Component
	{
	public:
		using Parametor = GoalAnimationController_Parametor;

		enum class TaskEnum {
			PreliminaryJump,	//ジャンプの溜め
			DunkMove,			//ダンクするところまで移動
			DunkWait,			//ダンク後の待機
			ReturnMove,			//ダンク後の元の位置に戻る処理
			EndWait,			//着地後の硬直
		};

	private:
		Parametor m_param;		//パラメータ

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスク管理

	public:
		GoalAnimationController(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;

	private:
		/// <summary>
		/// 開始アニメーションイベント
		/// </summary>
		void StartAnimationEvent();

		/// <summary>
		/// 更新時アニメーションイベント
		/// </summary>
		/// <returns>アニメーションが終了しているかどうか</returns>
		bool UpdateAnimationEvent();

		/// <summary>
		/// 終了時アニメーションイベント
		/// </summary>
		void ExitAnimationEvent();

		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		void SelectTask();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ダンク位置の設定
		/// </summary>
		/// <param name="position">ダンク位置</param>
		void SetDunkPosition(const Vec3& position) noexcept { m_param.dunkPosition = position; }

		/// <summary>
		/// ダンク位置の取得
		/// </summary>
		/// <returns>ダンク位置</returns>
		Vec3 GetDunkPosition() const noexcept { return m_param.dunkPosition; }
	};

}