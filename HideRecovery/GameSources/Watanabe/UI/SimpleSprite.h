﻿#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class SimpleSprite :public UIObjectBase {
	public:
		enum class Type {
			Texture,	// テクスチャ
			SpriteData,	// スプライトデータ
			None,		// なし（後で設定）
		};
	private:
		weak_ptr<PCTSpriteDraw> m_spriteDraw;
		shared_ptr<MeshResource> m_spriteMesh;
		Type m_type;
		wstring m_key;
	public:
		SimpleSprite(const shared_ptr<Stage>& stage);
		SimpleSprite(const shared_ptr<Stage>& stage, Type type, const wstring& key);
		SimpleSprite(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;

		void ChangeSprite(Type type, const wstring& key);
		shared_ptr<PCTSpriteDraw> GetDrawComponent();
	};
}
