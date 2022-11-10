/*!
@file   InProcessUI.h
@brief  処理中を示すUI
*/

#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SplashMessageUI.h"

namespace basecross {
	class InProcessUI :public UIObjectBase {
		using Type = SimpleSprite::Type;
	public:
		/// <summary>
		/// 表示するラベルタイプ
		/// </summary>
		enum class LabelType {
			Matching,	// マッチング
			Busy,		// 通信中
		};
	private:
		// ラベルに対応するデータマップ
		static map<LabelType, Message::MessageData> sm_Type2DataMap;
		// ラベルオブジェクト
		shared_ptr<SimpleSprite> m_label;
		// 処理中のぐるぐる
		shared_ptr<SimpleSprite> m_shape;
		// m_shapeのRectTransform
		weak_ptr<RectTransform> m_shapeRectTrans;
		// デフォルトの色
		Col4 m_defaultColor;
		// 時間計測用
		float m_totalDelta = 0.0f;
	public:
		InProcessUI(const shared_ptr<Stage>& stage);
		InProcessUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;

		/// <summary>
		/// 表示するラベルを設定
		/// </summary>
		/// <param name="messageType">ラベルタイプ</param>
		void SetLabel(LabelType messageType);
	};
}
