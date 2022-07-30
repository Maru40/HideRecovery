/*!
@file MainCamera.h
@brief メインカメラヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	メインカメラ
	//--------------------------------------------------------------------------------------
	class MainCamera : public Camera
	{
		std::weak_ptr<GameObject> m_target; //ターゲットのゲームオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		MainCamera(const std::weak_ptr<GameObject>& target);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~MainCamera() {}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットのセット
		/// </summary>
		/// <param name="target">ターゲット―のゲームオブジェクト</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

	};

}