#pragma once
#include "stdafx.h"
#include "AdvBaseDraw.h"
#include "ProjectShader.h"

namespace basecross {
	class BarrierShader : public AdvBaseDraw {
	public:
		explicit BarrierShader(const shared_ptr<GameObject>& owner);
		virtual ~BarrierShader();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
	};
}
