/*!
@file BoostEffectManager.h
@brief BoostEffectManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoostEffectObject;

	//--------------------------------------------------------------------------------------
	/// ブーストエフェクト管理
	//--------------------------------------------------------------------------------------
	class BoostEffectManager : public Component
	{
	private:
		ex_weak_ptr<BoostEffectObject> m_effectObject;  //エフェクトオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		BoostEffectManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 生成する場所を計算して取得
		/// </summary>
		/// <returns>生成する場所</returns>
		Vec3 CalculatePosition();

	public:
		/// <summary>
		/// エフェクトの生成
		/// </summary>
		void CreateEffect();

	};

}