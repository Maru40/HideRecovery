/*!
@file PNTToonDraw.cpp
@brief PNTToonDrawなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "CSMarchingCubeManager.h"

namespace basecross {

	//コンスタントバッファの実体部
	IMPLEMENT_DX11_CONSTANT_BUFFER(CSMarchingCubeBuffer);

	CSMarchingCubeManager::CSMarchingCubeManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void CSMarchingCubeManager::OnCreate() {
		//関係ないtest--------------------------------

		auto draw = GetGameObject()->AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");

		//--------------------------------------------

		//Element構造体に合わせたテンプレート
		InitializeBuffer<Element>();
	}

	void CSMarchingCubeManager::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();
		m_TotalTime += delta;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}

		CSUpdate();

		Element element;
		if (ReadElement(element)) { //リードできていたら
			//Vec3 resultPosition(0.0f);
			//resultPosition.x = element.pos.x;
			//resultPosition.y = element.pos.y;
			//resultPosition.z = element.pos.z;
			//transform->SetPosition(resultPosition);
		}
	}

	bool CSMarchingCubeManager::ReadElement(Element& element) {
		//デバイスの取得
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//結果の読み取り
		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		//読み出し用のインターフェースにm_Bufferをコピーする。
		pID3D11DeviceContext->CopyResource(m_ReadBackBuffer.Get(), m_OutBuffer.Get());
		//読み出しようのインターフェースをmapする。
		HRESULT hr = pID3D11DeviceContext->Map(
			m_ReadBackBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
		//読み出しに成功したら....
		if (SUCCEEDED(hr))
		{
			//マップされたリソースから、CPU側のデータにコピー(elemDataへ)
			memcpy(&element, mappedResource.pData, sizeof(Element)); //ここのサイズが変わる
			//マップの解放
			pID3D11DeviceContext->Unmap(m_ReadBackBuffer.Get(), 0);
		}

		return SUCCEEDED(hr);
	}

	std::shared_ptr<CSMarchingCubeManager::Element> CSMarchingCubeManager::ReadElement() {
		//エレメントをポインタで返す
		Element element;
		return (ReadElement(element)) ? std::make_shared<Element>(element) : nullptr;
	}

	void CSMarchingCubeManager::CSUpdate() {
		ConstantsUpdate();
		ElementInput();

		//デバイスの取得
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
		//CSの設定
		pID3D11DeviceContext->CSSetShader(CSMarchingCube::GetPtr()->GetShader(), nullptr, 0);
		//シェーダ―リソースビューの設定
		pID3D11DeviceContext->CSSetShaderResources(0, 1, m_SRV.GetAddressOf());
		//アクセスビューの設定
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), nullptr);
		//CSの実行
		pID3D11DeviceContext->Dispatch(m_numThread, 1, 1); //複数使いたい場合は(10, 1, 1)のようになる。(スレッド数)

		//CSの解放
		ID3D11UnorderedAccessView* ppUAViewNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, nullptr);
		ID3D11ShaderResourceView* ppSRVNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetShaderResources(0, 1, ppSRVNULL);
	}

	void CSMarchingCubeManager::ConstantsUpdate() {
		//デバイスの取得
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//コンスタントバッファの設定
		CSMarchingCubeConstants constans = {};
		SettingConstants(constans); //コンスタンスのセッティング
		//実際に設定する。
		ID3D11Buffer* pConstantBuffer = CSMarchingCubeBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &constans, 0, 0);
		pID3D11DeviceContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
	}

	void CSMarchingCubeManager::SettingConstants(CSMarchingCubeConstants& constans) {
		constans.paramf[0] = m_TotalTime;
	}

	void CSMarchingCubeManager::ElementInput() {
		//デバイスの取得
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//エレメントのデータ生成
		Element elemData;
		
		//Vec4 pos4(transform->GetPosition(), 0);
		//elemData.pos = pos4;

		//エレメントの入力
		pID3D11DeviceContext->UpdateSubresource(m_InBuffer.Get(), 0, nullptr, &elemData, 0, 0);
	}
}