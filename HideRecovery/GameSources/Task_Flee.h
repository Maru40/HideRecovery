
/*!
@file Task_Flee.h
@brief Task_Fleeなど
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

	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace Task {

			//--------------------------------------------------------------------------------------
			/// 逃亡タスクパラメータ
			//--------------------------------------------------------------------------------------
			struct Flee_Parametor {
				float maxSpeed;       //最大スピード
				float time;           //逃げる時間
				bool isTimer = true;  //時間経過で終了するかどうか

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Flee_Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="maxSpeed">最大スピード</param>
				Flee_Parametor(const float& maxSpeed);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="maxSpeed">最大スピード</param>
				/// <param name="time">逃げる時間</param>
				/// <param name="isTimer">時間経過で終了するかどうか</param>
				Flee_Parametor(const float& maxSpeed, const float& time, const bool isTimer = true);
			};

			//--------------------------------------------------------------------------------------
			/// 逃亡タスク
			//--------------------------------------------------------------------------------------
			class Flee : public TaskNodeBase<EnemyBase>
			{
			public:
				using Parametor = Flee_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //パラメータ

				std::unique_ptr<GameTimer> m_timer;  //タイム管理クラス

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				Flee(const std::shared_ptr<EnemyBase>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// 移動更新
				/// </summary>
				void MoveUpdate();

				/// <summary>
				/// 回転更新
				/// </summary>
				void RotationUpdate();

			public:
				/// <summary>
				/// 終了判断
				/// </summary>
				/// <returns>逃亡が完了したらtrue</returns>
				bool IsEnd();

			};

		}

	}
}