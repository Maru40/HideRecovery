/*!
@file RotationController.h
@brief RotationController
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	回転コントロール
	//--------------------------------------------------------------------------------------
	class RotationController : public Component
	{
	public:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			float speed;          //回転速度
 			Vec3 direction;          //向きたい方向
			bool isUseY = false;  //Y軸を考慮するかどうか

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">回転速度</param>
			/// <param name="isUseY">Y軸を考慮するかどうか</param>
			Parametor(const float& speed, const bool isUseY = false);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">回転速度</param>
			/// <param name="direction">向きたい方向</param>
			/// <param name="isUseY">Y軸を考慮するかどうか</param>
			Parametor(const float& speed, const Vec3& direction, const bool isUseY = false);
		};

	private:
		Parametor m_param;	//パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		RotationController(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 回転中かどうかを返す
		/// </summary>
		/// <returns>回転中ならtrue</returns>
		bool IsRotation();

		/// <summary>
		/// 向きたい方向を設定
		/// </summary>
		/// <param name="direction">向きたい方向</param>
		void SetDirection(const Vec3& direction) noexcept { m_param.direction = direction; }

		/// <summary>
		/// 向きたい方向の取得
		/// </summary>
		/// <returns>向きたい方向</returns>
		Vec3 GetDirect() const noexcept { return m_param.direction; }

		/// <summary>
		/// 回転速度の設定
		/// </summary>
		/// <param name="speed">回転速度</param>
		void SetSpeed(const float& speed) noexcept { m_param.speed = speed; }

		/// <summary>
		/// 回転速度の取得
		/// </summary>
		/// <returns>回転速度</returns>
		float GetSpeed() const noexcept { return m_param.speed; }

		/// <summary>
		/// Y軸を考慮するかどうかの設定
		/// </summary>
		/// <param name="isUseY">Y軸を考慮するならtrue</param>
		void SetIsUseY(const bool isUseY) noexcept { m_param.isUseY = isUseY; }

		/// <summary>
		/// Y軸を考慮するかどうかを返す
		/// </summary>
		/// <returns>Y軸を考慮するならtrue</returns>
		bool IsUseY() const noexcept { return m_param.isUseY; }

		/// <summary>
		/// パラメータの設定
		/// </summary>
		/// <param name="parametor">パラメータ</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// パラメータの取得
		/// </summary>
		/// <returns>パラメータ</returns>
		Parametor GetParametor() const noexcept { return m_param; }
	};

}

//endbasecross