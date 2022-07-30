
/*!
@file Task_Preliminary_Shoot.h
@brief Task_Preliminary_Shootなど
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
				/// ショット攻撃の予備動作パラメータ
				//--------------------------------------------------------------------------------------
				struct Preliminary_Shoot_Parametor
				{
					std::shared_ptr<ShootChargeData> chargeDataPtr; //チャージデータ

					/// <summary>
					/// コンストラクタ
					/// </summary>
					Preliminary_Shoot_Parametor();

					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="chargeDataPtr">チャージデータ</param>
					Preliminary_Shoot_Parametor(const std::shared_ptr<ShootChargeData>& chargeDataPtr);
				};

				//--------------------------------------------------------------------------------------
				/// ショット攻撃の予備動作タスク
				//--------------------------------------------------------------------------------------
				class Preliminary_Shoot : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = Preliminary_Shoot_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //パラメーター

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するクラス</param>
					/// <param name="paramPtr">パラメータ</param>
					Preliminary_Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// 向きの更新処理
					/// </summary>
					void Rotation();

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