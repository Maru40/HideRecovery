#pragma once
#include "stdafx.h"

namespace basecross {
	class DeviceResUtil {
	private:
		// 最大MSAAx8まで
		static constexpr UINT MAX_SAMPLE_COUNT = 8u;
		static DXGI_SAMPLE_DESC sm_SampleDescForMSAA;
	public:
		static DXGI_SAMPLE_DESC CheckMultisampleQualityLevels(const ComPtr<ID3D11Device>& device);
		static void CreateSwapChain(const ComPtr<ID3D11Device>& device, DXGI_SWAP_CHAIN_DESC& swapChainDesc, ComPtr<IDXGISwapChain>& swapChain);
	};
}
