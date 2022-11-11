/*!
@file   StaticModelDraw.h
@brief  静的モデルの描画コンポーネント
*/

#pragma once
#include "stdafx.h"
#include "AdvBaseDraw.h"
#include "ProjectShader.h"

namespace basecross {
	class StaticModelDraw : public AdvBaseDraw {
	public:
		explicit StaticModelDraw(const shared_ptr<GameObject>& owner);
		virtual ~StaticModelDraw();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
	};
}
