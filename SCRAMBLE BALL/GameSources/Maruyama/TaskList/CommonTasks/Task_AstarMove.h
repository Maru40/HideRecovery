/*!
@file Task_AstarMove
@brief Task_AstarMoveなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor;

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// Astar移動タイプ
		//--------------------------------------------------------------------------------------
		enum class AstarMove_Type
		{
			Chase,  //置きかける。
			Evade,  //逃げる。
		};

		//--------------------------------------------------------------------------------------
		/// Astar移動パラメータ
		//--------------------------------------------------------------------------------------
		struct AstarMove_Parametor 
		{
			using MoveType = AstarMove_Type;

			MoveType moveType;													//移動タイプ
			std::shared_ptr<Task_MovePositions_Parametor> movePositonsParamPtr; //移動パラメータ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="moveType">移動タイプ</param>
			AstarMove_Parametor(const MoveType& moveType);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="moveType">移動タイプ</param>
			/// <param name="movePositionsParamPtr">移動パラメータ</param>
			AstarMove_Parametor(
				const MoveType& moveType, 
				const std::shared_ptr<Task_MovePositions_Parametor>& movePositionsParamPtr);
		};

		//--------------------------------------------------------------------------------------
		/// Astarを利用した移動タスク
		//--------------------------------------------------------------------------------------
		class AstarMove : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AstarMove_Parametor;
			using MoveType = AstarMove_Type;

			/// <summary>
			/// タスクタイプ
			/// </summary>
			enum class TaskEnum {
				Move,   //移動
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr;               //パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
			/// <param name="paramPtr">パラメータ</param>
			AstarMove(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 向き更新
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

			/// <summary>
			/// 移動ルートをセットする。
			/// </summary>
			void SetRoutePositions();

		public:

		};

	}

}

//endbasecross