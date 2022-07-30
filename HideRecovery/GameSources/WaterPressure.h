/*!
@file WaterPressure.h
@brief ジェットポンプなど
担当：今泉志旺
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// 水圧ジェットの弾クラス
	//-----------------------------------------------------------------------------------------------
	class WaterPressure : public GameObject
	{
		Vec3 m_StartPos = Vec3(0); //! 発射の初期位置
		Vec3 m_MoveDir = Vec3(0); //! 動く方向

		float m_MovePow = 30.0f; //! 動く速さ
		float m_PushPower = 10.0f; //! 物を押す強さ

		float m_Weight = 1.0f; //! 横幅
		float m_Height = 1.0f; //! 縦幅

		ex_weak_ptr<GameObject> m_aura; //! エフェクトのポインタ

	public:
		//-----------------------------------------------------------------------------------------------
		/// 引数無しコンストラクタ
		WaterPressure(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		/**
		* @brief コンストラクタ
		* @param[in] startPos 初期位置
		* @param[in] moveDirection 動く方向
		*/
		WaterPressure(const std::shared_ptr<Stage>& stage, const Vec3& startPos, Vec3 moveDirection)
			:GameObject(stage), m_StartPos(startPos), m_MoveDir(moveDirection.normalize())
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		//-----------------------------------------------------------------------------------------------
		/// 位置の移動
		void Move();

		/// カメラの方向に向かせる
		void CapturedCamera();

		///自身を消去		
		void Erasure();

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------
		/// 物を押す強さの取得
		float GetPushPower() { return m_PushPower; }
		//-----------------------------------------------------------------------------------------------
	};
}
//endbasecross