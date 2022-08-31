/*!
@file RayMarchingFilterObject.h
@brief RayMarchingFilterObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// レイマーチングフィルターオブジェクト
	//--------------------------------------------------------------------------------------
	class RayMarchingFilterObject : public GameObject
	{
		ex_weak_ptr<Camera> m_target;  //ターゲット

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスの所属するステージ</param>
		RayMarchingFilterObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// デバッグドロー
		/// </summary>
		void DebugDraw();

		/// <summary>
		/// 追従設定
		/// </summary>
		void SettingSeek();
	};


}

//endbaescross