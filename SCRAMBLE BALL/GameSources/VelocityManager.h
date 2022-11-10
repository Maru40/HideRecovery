/*!
@file VelocityManager.h
@brief VelocityManager
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	速度管理
	//--------------------------------------------------------------------------------------
	class VelocityManager : public Component
	{
	public :
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			Vec3 force;					//加える力
			Vec3 velocity;				//速度       
			Vec3 beforeVelocity;		//位置フレーム前の速度

			float deselerationPower;	//減速速度
			float maxSpeed;				//最大スピード
			bool isDeseleration;		//減速中かどうか

			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="force">加える力</param>
			/// <param name="velocity">速度</param>
			/// <param name="deselerationPower">減速速度</param>
			/// <param name="isDeseleration">減速中かどうか</param>
			Parametor(const Vec3& force, const Vec3& velocity, const float& deselerationPower, const bool isDeseleration = false);
		};

	private:
		Parametor m_param;	//パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		VelocityManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		/// <summary>
		/// 移動処理
		/// </summary>
		void Move();

		/// <summary>
		/// 減速処理
		/// </summary>
		void Deseleration();

		/// <summary>
		/// 重力処理更新
		/// </summary>
		void GravityUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 速度の設定
		/// </summary>
		/// <param name="velocity">速度</param>
		void SetVelocity(const Vec3& velocity) noexcept;

		/// <summary>
		/// 速度の取得
		/// </summary>
		/// <returns>速度</returns>
		Vec3 GetVelocity() const noexcept;

		/// <summary>
		/// ワールド速度の取得
		/// </summary>
		/// <returns>ワールド速度</returns>
		Vec3 GetWorldVelocity() const;

		/// <summary>
		/// 1フレーム前の速度
		/// </summary>
		/// <returns>1フレーム前の速度</returns>
		Vec3 GetBeforeVelocity() const noexcept;

		/// <summary>
		/// 力を加える
		/// </summary>
		/// <param name="force">追加する速度</param>
		void AddForce(const Vec3& force) noexcept;

		/// <summary>
		/// 加える力の取得
		/// </summary>
		/// <returns>加える力</returns>
		Vec3 GetForce() const noexcept;

		/// <summary>
		/// 速度のリセット
		/// </summary>
		void ResetVelocity() noexcept;

		/// <summary>
		/// 加える力のリセット
		/// </summary>
        void ResetForce() noexcept;

		/// <summary>
		/// 速度と加える力のリセット
		/// </summary>
        void ResetAll() noexcept;

        /// <summary>
        /// 減速開始
        /// </summary>
        /// <param name="power">減速する力</param>
        void StartDeseleration(const float& power = 1.0f) noexcept;

		/// <summary>
		/// 減速しているかどうかの設定
		/// </summary>
		/// <param name="isDeseleration">減速しているかどうか</param>
        void SetIsDeseleration(const bool isDeseleration) noexcept;

		/// <summary>
		/// 減速しているかどうか
		/// </summary>
		/// <returns>減速している状態ならtrue</returns>
		bool IsDeseleration() const noexcept;

        /// <summary>
        /// 減速の強さ
        /// </summary>
		float GetDeselerationPower() const noexcept;

		/// <summary>
		/// 減速の強さ
		/// </summary>
		void SetDeseletationPower(const float& power) noexcept;

		/// <summary>
		/// 最大速度
		/// </summary>
		/// <param name="speed">最大速度</param>
		void SetMaxSpeed(const float& speed) noexcept;

		/// <summary>
		/// 最大速度の取得
		/// </summary>
		/// <returns>最大速度</returns>
		float GetMaxSpeed() const noexcept;
	};

}

//endbasecross