/*!
@file Task_PlowlingMove.h
@brief Task_PlowlingMoveなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	template<class T>
	class TaskList;

	namespace Enemy {
		class EnemyBase;

		//--------------------------------------------------------------------------------------
		/// スライムの徘徊移動タスク
		//--------------------------------------------------------------------------------------
		class Task_PlowlingMove : public TaskNodeBase<EnemyBase>
		{
		public:
			/// <summary>
			/// タスクタイプ
			/// </summary>
			enum class TaskEnum;

			/// <summary>
			/// パラメータ
			/// </summary>
			struct Parametor
			{
				std::shared_ptr<Task::ToTargetMove::Parametor> moveParamPtr; //移動パラメータ

				Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="moveParamPtr">移動パラメータ</param>
				Parametor(const std::shared_ptr<Task::ToTargetMove::Parametor>& moveParamPtr);
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr;              //パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList; //タスク管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するクラス</param>
			/// <param name="paramPtr">パラメータ</param>
			Task_PlowlingMove(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 向きの更新
			/// </summary>
			void Rotation();

			/// <summary>
			/// タスクの定義
			/// </summary>
			void DefineTask();

			/// <summary>
			/// タスクの選択
			/// </summary>
			void SelectTask();
		};

	}
}