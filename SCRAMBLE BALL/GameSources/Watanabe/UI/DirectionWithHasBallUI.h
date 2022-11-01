/*!
@file   DirectionWithHasBallUI.h
@brief  画面外のオブジェクト位置を指すUI
*/

#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"

namespace basecross {
	class DirectionWithHasBallUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_cursor;
		shared_ptr<Camera> m_camera;
		shared_ptr<ViewBase> m_view;
		shared_ptr<RectTransform> m_selfRectTrans;
		shared_ptr<Transform> m_targetTransform;
		Rect2D<float> m_screenRect;

		shared_ptr<SimpleSprite> CreateSprite(const wstring& spriteKey);
	public:
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;

		/// <summary>
		/// ターゲットをセット
		/// </summary>
		/// <param name="targetTransform">ターゲットのトランスフォーム</param>
		void SetTarget(const shared_ptr<Transform>& targetTransform);

		/// <summary>
		/// ターゲットをセット
		/// </summary>
		/// <param name="targetObject">ターゲットオブジェクト</param>
		void SetTarget(const shared_ptr<GameObject>& targetObject);

		/// <summary>
		/// ターゲット情報をクリア
		/// </summary>
		void ClearTarget();
	};
}
