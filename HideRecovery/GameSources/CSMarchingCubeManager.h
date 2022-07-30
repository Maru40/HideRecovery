#pragma once
/*!
@file ToonPNTStaticModelDraw.h
@brief ToonPNTStaticModelDraw
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//�R���X�^���g�o�b�t�@----------------------------------------------------------

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

		// �G�������g�f�[�^�̍\����
		struct Element
		{
			Vec3 position;
			Vec3 normal;
			Vec2 texcord;
			Vec2 indices;
			UINT useVertex;
			UINT useIndex;
		};

		//�G�������g�o�b�t�@
		ComPtr<ID3D11Buffer> m_InBuffer = nullptr;   //���͗p�o�b�t�@
		ComPtr<ID3D11Buffer> m_OutBuffer = nullptr;  //�o�͗p�o�b�t�@
		//�A�N�Z�X�r���[
		ComPtr < ID3D11UnorderedAccessView>  m_UAV = nullptr;
		//�V�F�[�_�[���\�[�X�r���[
		ComPtr<ID3D11ShaderResourceView> m_SRV = nullptr;
		//���[�h�o�b�N�o�b�t�@
		ComPtr<ID3D11Buffer> m_ReadBackBuffer = nullptr;

		//�G�������g�̐�
		const UINT NUM_ELEMENTS = 1;
		//�X���b�h��
		UINT m_numThread = 1;

		//�g�[�^���A�C��
		float m_TotalTime = 0.0f;

	public:
		CSMarchingCubeManager(const std::shared_ptr<GameObject>& objPtr);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	private:

		//�n�����f�[�^�ɒ��ڏ�������
		//�߂�l�͐����������ǂ���
		bool ReadElement(Element& element);
		//�V�F�[�_�\�Ōv�Z�����f�[�^�̎󂯎��
		//���s������nullptr��Ԃ��B
		std::shared_ptr<Element> ReadElement();

		//�R���s���[�g�V�F�[�_�̃A�b�v�f�[�g-----------------------------------------

		//�{��
		void CSUpdate();

		//�R���X�^���g�o�b�t�@�̃A�b�v�f�[�g
		void ConstantsUpdate();
		//�R���X�^���g�̃Z�b�e�B���O(�n�����f�[�^�ɒ��ڑ��)
		void SettingConstants(CSMarchingCubeConstants& constans);

		//�G�������g�̃C���v�b�g
		void ElementInput();

		//---------------------------------------------------------------------------

		//�e�o�b�t�@�̐���&&�����ݒ�
		template<class ElementType>
		void InitializeBuffer() {
			auto dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = dev->GetD3DDevice();
			auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

			//�G�������g�o�b�t�@����
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(ElementType);  //�o�b�t�@�ɓn���\���̂̃T�C�Y
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			buffer_desc.StructureByteStride = sizeof(ElementType);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_InBuffer),
				L"m_InBuffer�쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_OutBuffer),
				L"m_OutBuffer�쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);

			//�V�F�[�_�\���\�[�X�r���[
			D3D11_SHADER_RESOURCE_VIEW_DESC srvbuffer_desc = {};
			srvbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
			srvbuffer_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvbuffer_desc.Buffer.FirstElement = 0;
			srvbuffer_desc.Buffer.NumElements = buffer_desc.ByteWidth / buffer_desc.StructureByteStride;
			//srvbuffer_desc.Buffer.ElementWidth = NUM_ELEMENTS;
			srvbuffer_desc.Buffer.ElementWidth = NUM_ELEMENTS; //�z��̏ꍇ�͔z��
			ThrowIfFailed(
				pDx11Device->CreateShaderResourceView(m_InBuffer.Get(), &srvbuffer_desc, &m_SRV),
				L"m_SRV�쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateShaderResourceView()",
				L"PointsBall::OnCreate()"
			);

			//�A�N�Z�X�r���[����
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavbuffer_desc = {};
			uavbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
			uavbuffer_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavbuffer_desc.Buffer.NumElements = NUM_ELEMENTS;
			ThrowIfFailed(
				pDx11Device->CreateUnorderedAccessView(m_OutBuffer.Get(), &uavbuffer_desc, &m_UAV),
				L"�A�N�Z�X�r���[�쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateUnorderedAccessView()",
				L"MoveBox::OnCreate()"
			);

			//���[�h�o�b�N�o�b�t�@����
			D3D11_BUFFER_DESC readback_buffer_desc = {};
			readback_buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(ElementType);
			readback_buffer_desc.Usage = D3D11_USAGE_STAGING;
			readback_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			readback_buffer_desc.StructureByteStride = sizeof(ElementType);
			ThrowIfFailed(
				pDx11Device->CreateBuffer(&readback_buffer_desc, nullptr, &m_ReadBackBuffer),
				L"���[�h�o�b�N�o�b�t�@�쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateBuffer()",
				L"MoveBox::OnCreate()"
			);
		}
	};

}