
/*!
@file Slime_Plowling.h
@brief Slime_Plowlingなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Task_PlowlingMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class MulchTaskList;

	template<class T>
	class TaskList;

	template<class T>
	class EnemyStateNodeBase;
	
	class TargetManager;
	class GameTimer;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムの徘徊行動ステート
			//--------------------------------------------------------------------------------------
			class Slime_Plowling : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// 徘徊タスクタイプ
				/// </summary>
				enum class TaskEnum;

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------
				struct Parametor
				{
					vector<Vec3> positions;                                                   //徘徊するポジション配列
					std::shared_ptr<Task_PlowlingMove::Parametor> moveParamPtr = nullptr;     //移動するパラメータ
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;  //待機時のパラメータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					Parametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="moveParamPtr">移動パラメータ</param>
					/// <param name="waitParamPtr">待機パラメータ</param>
					Parametor(const std::shared_ptr<Task_PlowlingMove::Parametor>& moveParamPtr, 
						const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
				};
				
			private:
				unique_ptr<TaskList<TaskEnum>> m_taskList; //タスクリストを管理するクラス
				unique_ptr<GameTimer> m_arriveTimer;       //到着時間を計測する。(一定時間内に到着できていなければ、ルート再構築)

				ex_weak_ptr<Parametor> m_paramPtr;         //パラメータ
				int m_index = 0;						   //現在のインデックス
				int m_addIndexDirect = 1;				   //インデックスを進める方向

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				/// <param name="paramPtr">パラメータ</param>
				Slime_Plowling(const std::shared_ptr<EnemyBase>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

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
				/// 次の目的地のインデックスに変更
				/// </summary>
				void NextIndex();

				/// <summary>
				/// 次の目的地に変更
				/// </summary>
				void NextPosition();

				/// <summary>
				/// 移動先を変更
				/// </summary>
				/// <param name="position">変更したい移動先</param>
				void ChangeMoveTargetPosition(const Vec3& position);

				/// <summary>
				/// Astarを利用してRandomRouteを計測
				/// </summary>
				void CalculateAstarRamdomPosition();

				/// <summary>
				/// 到着予測時間を計算して取得
				/// </summary>
				/// <param name="targetPosition">到着地点</param>
				/// <param name="speed">移動スピード</param>
				/// <returns>到着予測時間</returns>
				float CalculateArriveTime(const Vec3& targetPosition, const float& speed) const;

				//デバッグ-----------------------------------------------------------------------

				void ChangeDebugNodeColor(const Col4& color);
			};

		}
	}
}