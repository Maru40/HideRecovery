/*!
@file MoveAstar.h
@brief MoveAstarなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor;
	template<class T>
	class TaskList;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	MoveAstarのパラメータ
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Ex_Parametor
		{
			std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

			MoveAstar_Ex_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスク
		//--------------------------------------------------------------------------------------
		class MoveAstar_Ex : public TaskNodeBase<Enemy::EnemyBase>
		{
		public:
			using Parametor = MoveAstar_Ex_Parametor;

		private:
			enum class TaskEnum {
				Move,	//移動
			};

			const Parametor* const m_param;					//パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

		public:
			MoveAstar_Ex(
				const std::shared_ptr<Enemy::EnemyBase>& owner,
				const Parametor* const parametor
			);

			virtual ~MoveAstar_Ex() = default;

			void OnCreate() override;

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
			
		public:
			bool IsEnd() const;

		};

	}

}