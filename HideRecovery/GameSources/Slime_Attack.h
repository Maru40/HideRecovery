/*!
@file Slime_Attack.h
@brief Slime_Attackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Task_Preliminary_NormalSlime.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	namespace Enemy {

		class EnemyBase;

		class Task_BodyBlow;
		struct Task_BodyBlow::Parametor;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// 攻撃パラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Attack_Parametor {
				std::shared_ptr<Task_Preliminary_NormalSlime::Parametor> preliminaryParamPtr = nullptr;    //予備動作パラメータ
				std::shared_ptr<Task_BodyBlow::Parametor> bodyBlowParamPtr = nullptr;                      //ボディブローパラメータ
				std::shared_ptr<Task::Wait::Parametor> waitParamPtr = nullptr;                             //待機パラメータ

				Slime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			/// 攻撃ステート
			//--------------------------------------------------------------------------------------
			class Slime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Attack_Parametor;

				/// <summary>
				/// タスクタイプ
				/// </summary>
				enum class TaskEnum {
					Preliminary, //予備動作
					BodyBlow,    //体当たり
					Wait,        //攻撃後の硬直
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr;               //パラメータ

				std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="parametor">パラメータ</param>
				Slime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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
				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// 現在のタスクタイプの取得
				/// </summary>
				TaskEnum GetCurrentTask() const;

			};

		}
	}
}