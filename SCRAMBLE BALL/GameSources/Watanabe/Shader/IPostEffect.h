#pragma once
#include "stdafx.h"

namespace basecross {
	class IPostEffect {
	public:
		virtual void UpdateConstantBuffer() = 0;
		virtual ID3D11ComputeShader* GetShader() const = 0;
	};
}