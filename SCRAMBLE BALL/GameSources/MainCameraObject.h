/*!
@file MainCameraObj.h
@brief MainCameraObjヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//前方宣言
	class SeekTPS;

	//--------------------------------------------------------------------------------------
	///	メインカメラオブジェクト
	//--------------------------------------------------------------------------------------
	class MainCameraObject : public GameObject
	{
		std::shared_ptr<GameObject> m_target;  //ターゲットのオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="target">ターゲットのオブジェクト</param>
		MainCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& target);

		void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットのセット
		/// </summary>
		/// <param name="target">ターゲットのオブジェクト</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);
	};

}

//endbasecross