/*!
@file PressRefrectionManager.h
@brief PressRefrectionManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// 潰す反射管理
	//--------------------------------------------------------------------------------------
	class PressRefrectionManager : public Component
	{
	public:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			Vec3 velocity;           //速度
			float deselerationPower; //減速速度

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="velocity">速度</param>
			/// <param name="deselerationPower">減速速度</param>
			Parametor(const Vec3& velocity, const float& deselerationPower);
		};

	private:
		Parametor m_param;   //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PressRefrectionManager(const std::shared_ptr<GameObject>& objPtr);

	private:
		/// <summary>
		/// 反射するベクトルを返す。
		/// </summary>
		/// <param name="data">潰すデータ</param>
		/// <returns>反射したベクトル</returns>
		Vec3 CalcuReflectionVec(const PressData& data);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// リフレクション開始
		/// </summary>
		/// <param name="data">押しつぶしたときに必要なデータ</param>
		/// <param name="deselerationPower">減速力</param>
		void StartReflection(const PressData& data, const float& deselerationPower = 1.0f);
	};

}

//endbasecross