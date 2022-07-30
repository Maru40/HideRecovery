/*!
@file BoostEffectObject.h
@brief BoostEffectObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoostCenterObject;

	//--------------------------------------------------------------------------------------
	/// ブーストエフェクトオブジェクト
	//--------------------------------------------------------------------------------------
	class BoostEffectObject : public GameObject
	{
	private:
		ex_weak_ptr<BoostCenterObject> m_center;        //ブーストの中心オブジェクト

		std::vector<ex_weak_ptr<GameObject>> m_objects; //生成したエフェクトオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		BoostEffectObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnUpdate() override;

		void OnDestroy() override;

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 生成したエフェクトオブジェクトの配列を取得
		/// </summary>
		/// <returns>生成したエフェクトオブジェクトの配列</returns>
		std::vector<ex_weak_ptr<GameObject>> GetEffectObjects() const noexcept {
			return m_objects;
		}

	};

}