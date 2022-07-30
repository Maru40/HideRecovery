/*!
@file BigSlime_Attack.h
@brief BigSlime_Attackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	class GameTimer;

	namespace Task {
		struct Wait_Parametor;
	}

	namespace Enemy {

		class EnemyBase;

		namespace BigSlime {
			namespace Task {
				struct Preliminary_Attack_Parametor;
				struct BodyBlow_Parametor;
			}
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// 巨大スライムの攻撃パラメータ
			//--------------------------------------------------------------------------------------
			struct BigSlime_Attack_Parametor {

				std::shared_ptr<BigSlime::Task::Preliminary_Attack_Parametor> preliminaryParamPtr = nullptr;    //予備動作パラメータ
				std::shared_ptr<BigSlime::Task::BodyBlow_Parametor> bodyBlowParamPtr = nullptr;                 //ボディブローパラメータ
				std::shared_ptr<basecross::Task::Wait_Parametor> waitParamPtr = nullptr;                        //待機パラメータ

				BigSlime_Attack_Parametor();
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムの攻撃ステート
			//--------------------------------------------------------------------------------------
			class BigSlime_Attack : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigSlime_Attack_Parametor;

				//--------------------------------------------------------------------------------------
				/// タスクタイプ
				//--------------------------------------------------------------------------------------
				enum class TaskEnum {
					Preliminary, //予備動作
					BodyBlow,    //ボディーブロー
					Wait,        //攻撃後の硬直
				};

			private:
				ex_weak_ptr<Parametor> m_paramPtr;              //パラメータ

				std::unique_ptr<TaskList<TaskEnum>> m_taskList; //タスク管理クラス

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				/// <param name="paramPtr">パラメータ</param>
				BigSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

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