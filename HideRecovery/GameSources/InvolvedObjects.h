/*!
@file InvolvedObjects.h
@brief 巻き込まれるオブジェクト
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// タックルに巻き込まれるオブジェクトクラス
	//-----------------------------------------------------------------------------------------------
	class InvolvedObjects : public StageObjectBase
	{
		Vec3 m_StartPos; //! 生成位置

		int m_Endurance = 1; //! ぶつかったときに壊れる残りの回数

		Vec2 m_RangeFactor = Vec2(0.5f, 0.5f); //! ダメージを受ける範囲
		float m_RotationScope = 1.0f; //! 回転する中心からの最低距離

		ex_weak_ptr<GameObject> m_ColObj; //! ぶつかってきた対象

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief コンストラクタ
		* @param[in] startPosition 初期位置
		*/
		InvolvedObjects(const std::shared_ptr<Stage>& stage, const Vec3& startPosition)
			:StageObjectBase(stage, L""), m_StartPos(startPosition)
		{}

		/**
		* @brief コンストラクタ
		* @param[in] name オブジェクトの名前
		* @param[in] texture オブジェクトに使用するテクスチャ
		*/
		InvolvedObjects(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture = L"")
			:StageObjectBase(stage, name, texture), m_StartPos(Vec3(0.0f))
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief ダメージを与える
		* @param[in] damage 与えるダメージの大きさ
		*/
		void Damaged(int damage);

		/// 破壊する
		void Breaking();

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------
		/// ダメージを受ける大きさの取得
		Vec2 GetRangeFactor() { return m_RangeFactor; }

		/// 回転するための最低距離を取得
		float GetRotationScope() { return m_RotationScope; }
		//-----------------------------------------------------------------------------------------------
	};
}

//endbasecross