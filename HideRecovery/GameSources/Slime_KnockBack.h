
/*!
@file Slime_KnockBack.h
@brief Slime_KnockBackなど
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

	template<class T>
	class EnemyStateNodeBase;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムのノックバックパラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_KnockBack_Parametor
			{
				float maxPower = 10.0f;                                          //ノックバックの最大パワー
				std::function<void()> startAnimationFunction = nullptr;          //スタート時に切り替えたいアニメーション
				std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr;   //硬直パラメータ

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Slime_KnockBack_Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="waitParamPtr">硬直パラメータ</param>
				Slime_KnockBack_Parametor(const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr);
			};

			//--------------------------------------------------------------------------------------
			/// スライムのノックバックステート
			//--------------------------------------------------------------------------------------
			class Slime_KnockBack : public StateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_KnockBack_Parametor;

				/// <summary>
				/// タスクタイプ
				/// </summary>
				enum class TaskEnum {
					NotDeselerationWait,  //減速しない状態
					Wait,                 //硬直
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
				Slime_KnockBack(const std::shared_ptr<EnemyBase>& owner ,const std::shared_ptr<Parametor>& paramPtr);

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

			};

		}
	}
}