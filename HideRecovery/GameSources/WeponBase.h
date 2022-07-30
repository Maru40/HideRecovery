/*!
@file WeponBase.h
@brief WeponBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BulletBase;
	class GameTimer;

	enum class Wepon_ShotType {
		Hold, //ホールドショット
	};

	//--------------------------------------------------------------------------------------
	/// 装備基底クラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct WeponBase_Parametor {
		using ShotType = Wepon_ShotType;    //ウェポンタイプ

		ShotType shotType = ShotType::Hold; //ショットタイプ
		Vec3 positionOffset = Vec3(0.0f);   //位置オフセット
		float energyConsumed;				//エネルギー消費量
		float overheatCoolTime;				//オーバーヒートクールタイム
		bool isOverheat;					//オーバーヒート状態かどうか
		
		/// <summary>
		/// コンストラクタ
		/// </summary>
		WeponBase_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="type">ショットタイプ</param>
		/// <param name="positionOffset">位置オフセット</param>
		WeponBase_Parametor(const ShotType& type, const Vec3& positionOffset);
	};

	//--------------------------------------------------------------------------------------
	/// 装備基底クラス
	//--------------------------------------------------------------------------------------
	class WeponBase : public Component
	{
	public:
		using Parametor = WeponBase_Parametor;
		using ShotType = Wepon_ShotType;

	private:
		std::unique_ptr<GameTimer> m_timer;		//時間管理クラス

	protected:
		ex_weak_ptr<GameObject> m_holdBullet;	//固定バレット
		Parametor m_baseParam;					//基底パラメータ	

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		WeponBase(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		WeponBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		virtual void OnUpdate() override;

		/// <summary>
		/// 撃つ処理
		/// </summary>
		virtual void Shot();

		/// <summary>
		/// 撃ち終わった処理
		/// </summary>
		virtual void ShotEnd();

	protected:

		/// <summary>
		/// ホールド中に撃つ処理
		/// </summary>
		virtual void HoldShotUpdate() {};

		/// <summary>
		/// ホールド解除
		/// </summary>
		virtual void HoldShotEnd() {};

		/// <summary>
		/// エネルギー消費
		/// </summary>
		virtual void ReduceEnergy();

		/// <summary>
		/// バレットの生成する位置を取得
		/// </summary>
		/// <returns>生成する場所</returns>
		Vec3 CalculateCreateBulletPosition();

		/// <summary>
		/// オーバーヒート
		/// </summary>
		void Overhear();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ショットタイプの取得
		/// </summary>
		/// <returns>ショットタイプ</returns>
		ShotType GetShotType() const noexcept { return m_baseParam.shotType; }

		/// <summary>
		/// 消費エネルギーの設定
		/// </summary>
		/// <param name="energyConsumed">消費エネルギー</param>
		void SetEnergyConsumed(const float energyConsumed) noexcept { m_baseParam.energyConsumed = energyConsumed; }

		/// <summary>
		/// 消費エネルギーの取得
		/// </summary>
		/// <returns>消費エネルギー</returns>
		float GetEnergyConsumed() const noexcept { return m_baseParam.energyConsumed; }

		/// <summary>
		/// オーバーヒートしているどうかを設定
		/// </summary>
		/// <param name="isOverheart">オーバーヒートしているかどうか</param>
		void SetIsOverheat(const bool isOverheart) { m_baseParam.isOverheat = isOverheart; }

		/// <summary>
		/// オーバーヒートしているかどうかを返す
		/// </summary>
		/// <returns>オーバーヒートしているならtrue</returns>
		bool IsOverheat() const noexcept { return m_baseParam.isOverheat; }

		/// <summary>
		/// エネルギーが使用可能かどうか
		/// </summary>
		/// <returns>エネルギー使用可能ならtrue</returns>
		bool IsUseEnergy() const;

		/// <summary>
		/// 撃てる状態かどうか
		/// </summary>
		/// <returns>撃てる状態ならtrue</returns>
		bool IsShot() const;

	};

}