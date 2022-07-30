
/*!
@file Task_Shoot.h
@brief Task_Shootなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace ShootSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// ショットタスクのパラメータ
				//--------------------------------------------------------------------------------------
				struct Shoot_Parametor 
				{
					float speed;      //弾を撃つ速度
					float backPower;  //反動の力

					/// <summary>
					/// コンストラクタ
					/// </summary>
					Shoot_Parametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="speed">弾を撃つ速度</param>
					/// <param name="backPower">反動の力</param>
					Shoot_Parametor(const float& speed, const float& backPower);
				};

				//--------------------------------------------------------------------------------------
				/// ショットタスク
				//--------------------------------------------------------------------------------------
				class Shoot : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Shoot_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //パラメータ

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するクラス</param>
					/// <param name="paramPtr">パラメータ</param>
					Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				public:
					//--------------------------------------------------------------------------------------
					/// アクセッサ
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// 終了判断
					/// </summary>
					/// <returns>更新処理が終了したらtrue</returns>
					bool IsEnd();

				};

			}
		}
	}
}