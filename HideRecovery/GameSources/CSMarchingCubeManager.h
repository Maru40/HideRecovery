#pragma once
/*!
@file ToonPNTStaticModelDraw.h
@brief ToonPNTStaticModelDraw
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//コンスタントバッファ----------------------------------------------------------

	struct CSMarchingCubeConstants
	{
		// paramf[0] == Totaltime
		float paramf[4];
	};

	DECLARE_DX11_CONSTANT_BUFFER(CSMarchingCubeBuffer, CSMarchingCubeConstants);

	//------------------------------------------------------------------------------

	
	class CSMarchingCubeManager : public Component
	{
		static constexpr UINT VertexNum = 300;
		static constexpr UINT Num = 5000;

		// エレメントデータの構造体
		struct Element
		{
			Vec3 position;
			Vec3 normal;
			Vec2 texcord;
			Vec2 indices;
			UINT useVertex;
			UINT useIndex;
		};

		//エレメントバッファ
		ComPtr<ID3D11Buffer> m_InBuffer = nullptr;   //入力用バッファ
		ComPtr<ID3D11Buffer> m_OutBuffer = nullptr;  //出力用バッファ
		//アクセスビュー
		ComPtr < ID3D11UnorderedAccessView>  m_UAV = nullptr;
		//シェーダーリソースビュー
		ComPtr<ID3D11ShaderResourceView> m_SRV = nullptr;
		//リードバックバッファ
		ComPtr<ID3D11Buffer> m_ReadBackBuffer = nullptr;

		//エレメントの数
		const UINT NUM_ELEMENTS = 1;
		//スレッド数
		UINT m_numThread = 1;

		//トータルアイム
		float m_TotalTime = 0.0f;

	public:
		CSMarchingCubeManager(const std::shared_ptr<GameObject>& objPtr);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	private:

		//渡したデータに直接書き込み
		//戻り値は成功したかどうか
		bool ReadElement(Element& element);
		//シェーダ―で計算したデータの受け取り
		//失敗したらnullptrを返す。
		std::shared_ptr<Element> ReadElement();

		//コンピュートシェーダのアップデート-----------------------------------------

		//本体
		void CSUpdate();

		//コンスタントバッファのアップデート
		void ConstantsUpdate();
		//コンスタントのセッティング(渡したデータに直接代入)
		void SettingConstants(CSMarchingCubeConstants& constans);

		//エレメントのインプット
		void ElementInput();

		//---------------------------------------------------------------------------

		//各バッファの生成&&初期設定
		template<class ElementType>
		void InitializeBuffer() {
			auto dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = dev->GetD3DDevice();
			auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

			//エレメントバッファ生成
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(ElementType);  //バッファに渡す構造体のサイズ
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			buffer_desc.StructureByteStride = sizeof(ElementType);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_InBuffer),
				L"m_InBuffer作成に失敗しました",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_OutBuffer),
				L"m_OutBuffer作成に失敗しました",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);

			//シェーダ―リソースビュー
			D3D11_SHADER_RESOURCE_VIEW_DESC srvbuffer_desc = {};
			srvbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
			srvbuffer_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvbuffer_desc.Buffer.FirstElement = 0;
			srvbuffer_desc.Buffer.NumElements = buffer_desc.ByteWidth / buffer_desc.StructureByteStride;
			//srvbuffer_desc.Buffer.ElementWidth = NUM_ELEMENTS;
			srvbuffer_desc.Buffer.ElementWidth = NUM_ELEMENTS; //配列の場合は配列数
			ThrowIfFailed(
				pDx11Device->CreateShaderResourceView(m_InBuffer.Get(), &srvbuffer_desc, &m_SRV),
				L"m_SRV作成に失敗しました",
				L"pDx11Device->CreateShaderResourceView()",
				L"PointsBall::OnCreate()"
			);

			//アクセスビュー生成
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavbuffer_desc = {};
			uavbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
			uavbuffer_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavbuffer_desc.Buffer.NumElements = NUM_ELEMENTS;
			ThrowIfFailed(
				pDx11Device->CreateUnorderedAccessView(m_OutBuffer.Get(), &uavbuffer_desc, &m_UAV),
				L"アクセスビュー作成に失敗しました",
				L"pDx11Device->CreateUnorderedAccessView()",
				L"MoveBox::OnCreate()"
			);

			//リードバックバッファ生成
			D3D11_BUFFER_DESC readback_buffer_desc = {};
			readback_buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(ElementType);
			readback_buffer_desc.Usage = D3D11_USAGE_STAGING;
			readback_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			readback_buffer_desc.StructureByteStride = sizeof(ElementType);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&readback_buffer_desc, nullptr, &m_ReadBackBuffer),
				L"リードバックバッファ作成に失敗しました",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);
		}
	};

}