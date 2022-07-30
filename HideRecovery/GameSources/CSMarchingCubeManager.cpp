/*!
@file PNTToonDraw.cpp
@brief PNTToonDraw�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "CSMarchingCubeManager.h"

namespace basecross {

	//�R���X�^���g�o�b�t�@�̎��̕�
	IMPLEMENT_DX11_CONSTANT_BUFFER(CSMarchingCubeBuffer);

	CSMarchingCubeManager::CSMarchingCubeManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void CSMarchingCubeManager::OnCreate() {
		//�֌W�Ȃ�test--------------------------------

		auto draw = GetGameObject()->AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");

		//--------------------------------------------

		//Element�\���̂ɍ��킹���e���v���[�g
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
		if (ReadElement(element)) { //���[�h�ł��Ă�����
			//Vec3 resultPosition(0.0f);
			//resultPosition.x = element.pos.x;
			//resultPosition.y = element.pos.y;
			//resultPosition.z = element.pos.z;
			//transform->SetPosition(resultPosition);
		}
	}

	bool CSMarchingCubeManager::ReadElement(Element& element) {
		//�f�o�C�X�̎擾
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//���ʂ̓ǂݎ��
		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		//�ǂݏo���p�̃C���^�[�t�F�[�X��m_Buffer���R�s�[����B
		pID3D11DeviceContext->CopyResource(m_ReadBackBuffer.Get(), m_OutBuffer.Get());
		//�ǂݏo���悤�̃C���^�[�t�F�[�X��map����B
		HRESULT hr = pID3D11DeviceContext->Map(
			m_ReadBackBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
		//�ǂݏo���ɐ���������....
		if (SUCCEEDED(hr))
		{
			//�}�b�v���ꂽ���\�[�X����ACPU���̃f�[�^�ɃR�s�[(elemData��)
			memcpy(&element, mappedResource.pData, sizeof(Element)); //�����̃T�C�Y���ς��
			//�}�b�v�̉��
			pID3D11DeviceContext->Unmap(m_ReadBackBuffer.Get(), 0);
		}

		return SUCCEEDED(hr);
	}

	std::shared_ptr<CSMarchingCubeManager::Element> CSMarchingCubeManager::ReadElement() {
		//�G�������g���|�C���^�ŕԂ�
		Element element;
		return (ReadElement(element)) ? std::make_shared<Element>(element) : nullptr;
	}

	void CSMarchingCubeManager::CSUpdate() {
		ConstantsUpdate();
		ElementInput();

		//�f�o�C�X�̎擾
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
		//CS�̐ݒ�
		pID3D11DeviceContext->CSSetShader(CSMarchingCube::GetPtr()->GetShader(), nullptr, 0);
		//�V�F�[�_�\���\�[�X�r���[�̐ݒ�
		pID3D11DeviceContext->CSSetShaderResources(0, 1, m_SRV.GetAddressOf());
		//�A�N�Z�X�r���[�̐ݒ�
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), nullptr);
		//CS�̎��s
		pID3D11DeviceContext->Dispatch(m_numThread, 1, 1); //�����g�������ꍇ��(10, 1, 1)�̂悤�ɂȂ�B(�X���b�h��)

		//CS�̉��
		ID3D11UnorderedAccessView* ppUAViewNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, nullptr);
		ID3D11ShaderResourceView* ppSRVNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetShaderResources(0, 1, ppSRVNULL);
	}

	void CSMarchingCubeManager::ConstantsUpdate() {
		//�f�o�C�X�̎擾
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		CSMarchingCubeConstants constans = {};
		SettingConstants(constans); //�R���X�^���X�̃Z�b�e�B���O
		//���ۂɐݒ肷��B
		ID3D11Buffer* pConstantBuffer = CSMarchingCubeBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &constans, 0, 0);
		pID3D11DeviceContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
	}

	void CSMarchingCubeManager::SettingConstants(CSMarchingCubeConstants& constans) {
		constans.paramf[0] = m_TotalTime;
	}

	void CSMarchingCubeManager::ElementInput() {
		//�f�o�C�X�̎擾
		auto dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();

		//�G�������g�̃f�[�^����
		Element elemData;
		
		//Vec4 pos4(transform->GetPosition(), 0);
		//elemData.pos = pos4;

		//�G�������g�̓���
		pID3D11DeviceContext->UpdateSubresource(m_InBuffer.Get(), 0, nullptr, &elemData, 0, 0);
	}
}