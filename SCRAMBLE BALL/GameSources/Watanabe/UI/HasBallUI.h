#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"

namespace basecross {
	class HasBallUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_icon;
		shared_ptr<ViewBase> m_view;
		shared_ptr<RectTransform> m_selfRectTrans;
		shared_ptr<Transform> m_targetTransform;
		Rect2D<float> m_screenRect;
		Vec2 m_offset;
		shared_ptr<SimpleSprite> CreateSprite(const wstring& spriteKey);
	public:
		HasBallUI(const shared_ptr<Stage>& stage);
		HasBallUI(const shared_ptr<Stage>& stage, const Vec2 offset);

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
