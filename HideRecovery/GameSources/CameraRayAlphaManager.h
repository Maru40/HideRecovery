/*!
@file CameraRayAlphaManager.h
@brief CameraRayAlphaManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	カメラからのRayでオブジェクトのAlphaを変更するコンポーネント
	//--------------------------------------------------------------------------------------
	class CameraRayAlphaManager : public Component
	{
		ex_weak_ptr<GameObject> m_target;   //ターゲット
		float m_range;						//透明化範囲

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲット</param>
		/// <param name="range">透明化範囲</param>
		CameraRayAlphaManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const float& range = 4.0f);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ターゲットの設定
		/// </summary>
		/// <param name="target">ターゲット</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) {
			m_target = target;
		}
	};

}

//endbasecross