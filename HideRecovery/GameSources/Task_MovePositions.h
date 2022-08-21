/*!
@file Task_MovePositions.h
@brief Task_MovePositions
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 徘徊移動するタスクのパラメータ
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor{
		std::vector<Vec3> positions;  //徘徊する目的地群
		std::shared_ptr<Task::Task_ToTargetMove::Parametor> moveParamPtr;
		bool isSetInitializePosition; //初期設定のポジションに瞬間移動させるかどうか

		Task_MovePositions_Parametor();
		Task_MovePositions_Parametor(const Task::Task_ToTargetMove::Parametor& moveParametor);
		Task_MovePositions_Parametor(const std::shared_ptr<Task::Task_ToTargetMove::Parametor>& moveParamPtr);
	};

	//--------------------------------------------------------------------------------------
	/// 徘徊移動するタスク
	//--------------------------------------------------------------------------------------
	class Task_MovePositions : public TaskNodeBase<GameObject>
	{
	public:
		using Parametor = Task_MovePositions_Parametor;

		/// <summary>
		/// タスクタイプ
		/// </summary>
		enum class TaskEnum {
			Move,  //移動
		};

		ex_weak_ptr<Parametor> m_paramPtr;               //パラメータ
		std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス

		int m_index = 0;                                 //現在の目的地のインデックス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
		/// <param name="paramPtr">パラメータ</param>
		Task_MovePositions(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

		void OnStart() override;
		bool OnUpdate() override;
		void OnExit() override;

	private:
		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		void SelectTask();

		/// <summary>
		/// 次の目的地に変更
		/// </summary>
		void NextPosition();

		/// <summary>
		/// 目的地を変更
		/// </summary>
		/// <param name="position">変更したい目的地</param>
		void ChangeMoveTargetPosition(const Vec3& position);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>移動先がないならtrue</returns>
		bool IsEnd();
 	};

}

//endbasecross