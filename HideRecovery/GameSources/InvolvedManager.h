/*!
@file InvolvedManager.h
@brief 巻き込まれるオブジェクトの衝突判定
*/

#pragma once
#include "stdafx.h"
#include "InvolvedObjects.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// 巻き込まれるオブジェクトの衝突判定コンポーネントクラス
	//-----------------------------------------------------------------------------------------------
	class InvolvedManager : public Component
	{
		std::shared_ptr<InvolvedObjects> m_InvolObj; //! 自身の追加先のオブジェクト
		std::shared_ptr<Transform> m_InvolTrans; //! 追加先のオブジェクトのトランスフォーム
		float m_RotationPow = 5.0f; //! 回転の速度

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief コンストラクタ
		* @param[in] involObj 追加先のオブジェクト
		*/
		InvolvedManager(const std::shared_ptr<GameObject>& ptrObj, const std::shared_ptr<InvolvedObjects>& involObj)
			:Component(ptrObj), m_InvolObj(involObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCollisionEnter(const CollisionPair& pair) override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief ダメージ判定
		* @param[in] point 衝突した位置
		*/
		void HitDamage(const Vec3& point);

		/**
		* @brief 回転処理
		* @param[in] hitCenter 衝突した位置
		*/
		void ObjectRotation(const Vec3& hitCenter);
		//-----------------------------------------------------------------------------------------------
	};
}