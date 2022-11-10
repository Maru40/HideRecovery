#include "stdafx.h"
#include "DeviceResUtil.h"

namespace basecross {
	DXGI_SAMPLE_DESC DeviceResUtil::sm_SampleDescForMSAA = { -1,-1 };

	DXGI_SAMPLE_DESC DeviceResUtil::CheckMultisampleQualityLevels(const ComPtr<ID3D11Device>& device) {
		if (sm_SampleDescForMSAA.Count != -1)
			return sm_SampleDescForMSAA;

		DXGI_SAMPLE_DESC sampleDesc = {};
		for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
		{
			UINT Quality;
			if (SUCCEEDED(device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
			{
				if (0 < Quality)
				{
					sampleDesc.Count = i;
					sampleDesc.Quality = Quality - 1;
				}
			}
		}
		sm_SampleDescForMSAA = sampleDesc;
		return sampleDesc;
	}

	void DeviceResUtil::CreateSwapChain(const ComPtr<ID3D11Device>& device, DXGI_SWAP_CHAIN_DESC& swapChainDesc, ComPtr<IDXGISwapChain>& swapChain) {
		IDXGIDevice1* hpDXGI;
		device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&hpDXGI));
		IDXGIAdapter* hpAdapter;
		hpDXGI->GetAdapter(&hpAdapter);
		IDXGIFactory* hpDXGIFactory;
		hpAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&hpDXGIFactory));
		hpDXGIFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
		hpDXGIFactory->Release();
		hpAdapter->Release();
		hpDXGI->Release();
	}
}