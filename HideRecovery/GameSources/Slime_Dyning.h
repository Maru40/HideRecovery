/*!
@file Slime_Dyning.h
@brief Slime_Dyningなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライム死亡遷移ステートのパラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Dyning_Parametor
			{
				float time;          //死亡するまでの時間
				float pressPower;    //潰す力
				PressData pressData; //PressData

				/// <summary>
				/// コンストラクタ
				/// </summary>
				Slime_Dyning_Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="time">死亡するまでの時間</param>
				/// <param name="pressPower">つぶされる力</param>
				/// <param name="pressData">つぶされるデータ</param>
				Slime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData);
			};

			//--------------------------------------------------------------------------------------
			/// スライム死亡遷移ステート
			//--------------------------------------------------------------------------------------
			class Slime_Dyning : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Dyning_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;   //パラメータ

				std::unique_ptr<GameTimer> m_timer = nullptr;  //タイム管理クラス

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				Slime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// SuccessEffectを生成
				/// </summary>
				void CreateSuccessEffect();
			};

		}
	}
}
