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
		weak_ptr<SimpleSprite> m_cursor;
		weak_ptr<Camera> m_camera;
		weak_ptr<ViewBase> m_view;
		weak_ptr<RectTransform> m_selfRectTrans;
		weak_ptr<Transform> m_targetTransform;
		Rect2D<float> m_screenRect;

	public:
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage);
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage, const wstring& line);

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

		/// <summary>
		/// ターゲットがセットされているか
		/// </summary>
		/// <returns>されていたらtrue</returns>
		bool IsTargetSet();
	};
}
