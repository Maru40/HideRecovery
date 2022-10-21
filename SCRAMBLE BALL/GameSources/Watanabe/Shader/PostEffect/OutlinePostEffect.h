#pragma once
#include "stdafx.h"
#include "../IPostEffect.h"

namespace basecross {
	struct OutlineConstructBuffer {
		Vec2 fogCoefficient;
		UINT windowWidth;
		UINT windowHeight;
		Col4 fogColor;
	};

	DECLARE_DX11_CONSTANT_BUFFER(OutlineCB, OutlineConstructBuffer)
	DECLARE_DX11_COMPUTE_SHADER(CS_Outline)

	class OutlinePostEffect : public IPostEffect {
	public:
		OutlinePostEffect();

		void UpdateConstantBuffer() override;
		ID3D11ComputeShader* GetShader() const override;
	};
}