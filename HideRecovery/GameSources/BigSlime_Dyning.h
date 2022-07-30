/*!
@file BigSlime_Dyning.h
@brief BigSlime_Dyningなど
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

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// 巨大スライムの死亡時パラメータ
			//--------------------------------------------------------------------------------------
			struct BigSlime_Dyning_Parametor
			{
				float time;          //潰す時間
				float pressPower;    //潰す力
				PressData pressData; //潰すデータ

				BigSlime_Dyning_Parametor();
				BigSlime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData);
			};

			//--------------------------------------------------------------------------------------
			///	巨大スライムの死亡時ステート
			//--------------------------------------------------------------------------------------
			class BigSlime_Dyning : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigSlime_Dyning_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;   //パラメータ。
				std::unique_ptr<GameTimer> m_timer = nullptr;  //時間管理クラス。

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
				/// <param name="paramPtr">パラメータ</param>
				BigSlime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
			};

		}
	}
}