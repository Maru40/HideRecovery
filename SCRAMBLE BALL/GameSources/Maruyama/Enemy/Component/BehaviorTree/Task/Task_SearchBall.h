/*!
@file Task_SearchBall.h
@brief Task_SearchBallなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {
	
	template<class T>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {
		struct MoveAstar_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				enum class TaskEnum {
					MoveAstar,	//Astarを利用してターゲットの近くまで移動する。
					MoveArrive,	//ターゲットが視界内なら到着行動
				};

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスクパラメータ
				//--------------------------------------------------------------------------------------
				struct SearchBall_Parametor {
					basecross::Task::MoveAstar_Parametor* moveAstarParam;
					std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

					SearchBall_Parametor();

					virtual ~SearchBall_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスク
				//--------------------------------------------------------------------------------------
				class SearchBall : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = SearchBall_Parametor;

					std::queue<int> m_areaRoute;	//確認するエリアルート

				private:
					Parametor m_param;								//パラメータ
					
					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

					std::weak_ptr<Transform> m_transform;

				public:
					SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:

					void DefineTask();

					void SelectTask();

					void NextRoute();

					std::queue<int> CalculateMoveAreaRouteQueue();

					/// <summary>
					/// 徘徊する場所の配列を取得
					/// </summary>
					/// <returns>徘徊する場所の配列</returns>
					std::vector<Vec3> CalculateMovePositions();

					/// <summary>
					/// 徘徊移動先の目的地を取得
					/// </summary>
					/// <returns>徘徊移動先を取得</returns>
					Vec3 CalculateMoveTargetPosition();

					/// <summary>
					/// パラメータの初期化
					/// </summary>
					void InitializeParametor();

				public:

					bool IsEnd() const;
				};

			}

		}
	}
}