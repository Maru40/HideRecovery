/*!
@file ChaseState_AstarSeek.h
@brief ChaseState_AstarSeekなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class EnemyStateNodeBase;

	template<class EnumType>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Task {
		struct AroundEyeCheck_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			namespace ChaseState {

				//--------------------------------------------------------------------------------------
				/// Astarを使った追従パラメータ
				//--------------------------------------------------------------------------------------
				struct AstarSeekParametor
				{
					std::shared_ptr<Task_MovePositions_Parametor> moveParamPtr;              //移動パラメータ
					std::shared_ptr<Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr;  //周囲を確認するときのパラメータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					AstarSeekParametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="moveParamPtr">移動パラメータ</param>
					/// <param name="aroundEyeCheckParamPtr">周囲を確認するパラメータ</param>
					AstarSeekParametor(const std::shared_ptr<Task_MovePositions_Parametor> moveParamPtr,
						const std::shared_ptr<Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// Astarを使った追従ステート
				//--------------------------------------------------------------------------------------
				class ChaseState_AstarSeek : public EnemyStateNodeBase<EnemyBase>
				{
				public:

					/// <summary>
					/// タスクタイプ
					/// </summary>
					enum class TaskEnum {
						Move,            //移動
						AroundMoveCheck, //周囲を移動して確認
						AroundEyeCheck,  //周囲を目で確認
					};

				private:
					ex_weak_ptr<AstarSeekParametor> m_paramPtr = nullptr;      //パラメータ

					std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;  //タスクリスト管理

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するクラス</param>
					/// <param name="paramPtr">パラメータ</param>
					ChaseState_AstarSeek(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<AstarSeekParametor>& paramPtr);

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
					/// 移動開始準備
					/// </summary>
					void ReserveMoveStart();

					/// <summary>
					/// 回転処理
					/// </summary>
					void Rotation();
					/// <summary>
					/// 終了時の処理
					/// </summary>
					void EndProcess(); 

				public:

					//--------------------------------------------------------------------------------------
					/// アクセッサ
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// 移動終了判断
					/// </summary>
					/// <returns>目的ノードに近づき、移動が終了したらtrue</returns>
					bool IsMoveEnd();
				};
			}
		}
	}
}