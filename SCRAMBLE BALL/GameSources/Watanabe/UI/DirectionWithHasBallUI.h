#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"

namespace basecross {
	class DirectionWithHasBallUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_cursor;
		shared_ptr<Camera> m_camera;
		shared_ptr<RectTransform> m_rectTrans;
		shared_ptr<Transform> m_targetObject;

		shared_ptr<SimpleSprite> CreateSprite(const wstring& spriteKey);
	public:
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;

		/// <summary>
		/// ターゲットをセット
		/// </summary>
		/// <param name="target">ターゲットのトランスフォーム</param>
		void SetTarget(const shared_ptr<Transform>& target);

		/// <summary>
		/// ターゲット情報をクリア
		/// </summary>
		void ClearTarget();
	};
}
