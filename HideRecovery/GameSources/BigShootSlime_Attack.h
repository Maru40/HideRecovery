/*!
@file BigShootSlime_Attack.h
@brief BigShootSlime_Attackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace BigShootSlime {
			namespace Task {
				struct Preliminary_Attack_Parametor;
				struct BodyBlow_Parametor;
			}
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	巨大シュートスライムの攻撃パラメータ
			//--------------------------------------------------------------------------------------
			struct BigShootSlime_Attack_Parametor {
				BigShootSlime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			///	巨大シュートスライムの攻撃ステート
			//--------------------------------------------------------------------------------------
			class BigShootSlime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigShootSlime_Attack_Parametor;

				//--------------------------------------------------------------------------------------
				///	タスクタイプ
				//--------------------------------------------------------------------------------------
				enum class TaskEnum {
					Preliminary, //予備動作
					BodyBlow,    //BodyBlow
					Wait,        //攻撃後の硬直時間
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;               //パラメータ。

				std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;  //タスクリストクラス。

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				/// <param name="paramPtr">パラメータ</param>
				BigShootSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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
				/// 終了判断
				/// </summary>
				/// <returns>更新終了条件を満たしたらtrue</returns>
				bool IsEnd() const;

			};

		}
	}
}