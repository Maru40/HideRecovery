
/*!
@file ShootManager.h
@brief ShootManagerなど
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

			//--------------------------------------------------------------------------------------
			/// ショット管理のパラメータ
			//--------------------------------------------------------------------------------------
			struct ShootManager_Parametor
			{

			};

			//--------------------------------------------------------------------------------------
			/// ショットチャージデータ
			//--------------------------------------------------------------------------------------
			struct ShootChargeData 
			{
				float time;  //チャージ時間
				float range; //移動距離

				/// <summary>
				/// コンストラクタ
				/// </summary>
				ShootChargeData();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="time">チャージ時間</param>
				/// <param name="range">移動距離</param>
				ShootChargeData(const float& time, const float& range);
			};

			//--------------------------------------------------------------------------------------
			/// ショット管理クラス
			//--------------------------------------------------------------------------------------
			class ShootManager : public Component
			{
			public:
				using Parametor = ShootManager_Parametor;

			private:
				Parametor m_param;                      //パラメータ

				ex_weak_ptr<GameObject> m_bulletObject; //バレットオブジェクト

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				ShootManager(const std::shared_ptr<GameObject>& objPtr);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				/// <param name="parametor">パラメータ</param>
				ShootManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

				void OnCreate() override;
				void OnDestroy() override;

			private:
				/// <summary>
				/// バレットオブジェクトを生成する
				/// </summary>
				void CrealteBulletObject();

			public:
				/// <summary>
				/// 中心に集める処理を開始する。
				/// </summary>
				/// <param name="time">チャージ時間</param>
				/// <param name="range">移動距離</param>
				void StartCharge(const float& time, const float& range);

				/// <summary>
				/// 中心に集める処理を開始する。
				/// </summary>
				/// <param name="data">チャージデータ</param>
				void StartCharge(const ShootChargeData& data);

				/// <summary>
				/// Shootを中断してBreakする処理
				/// </summary>
				void Break();

				/// <summary>
				/// チャージの終了を判断する。
				/// </summary>
				/// <returns>チャージが終了したらtrue</returns>
				bool IsEndCharge() const;

				/// <summary>
				/// 撃つ処理
				/// </summary>
				/// <param name="speed">移動スピード</param>
				/// <param name="target">撃つターゲット</param>
				void Shoot(const float& speed, const std::shared_ptr<GameObject>& target);

				/// <summary>
				/// 撃つ処理
				/// </summary>
				/// <param name="speed">移動スピード</param>
				/// <param name="targetPosition">ターゲットの位置</param>
				void Shoot(const float& speed, const Vec3& targetPosition);

				/// <summary>
				/// リロード
				/// </summary>
				void Reload();
			};

		}
	}
}