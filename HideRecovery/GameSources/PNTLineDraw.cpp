/*!
@file PNTLineDraw.cpp
@brief PNTLineDraw�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "PNTLineDraw.h"

namespace basecross {

	PNTLineDraw::PNTLineDraw(const std::shared_ptr<GameObject>& objPtr)
		:SmBaseDraw(objPtr)
	{}

	void PNTLineDraw::OnCreate() {
		//SetLightingEnabled(true);
		////�}���`���C�g�̐ݒ�
		//for (int i = 0; i < GetMaxDirectionalLights(); i++) {
		//	SetLightEnabled(i, true);
		//}

		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTLineDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}

		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic2(PtrMeshResource->GetMashData());
		}
		return;
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawStatic<VSPNTStatic, PSPNTStatic>(vec[i]);
					}
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}


	void PNTLineDraw::DrawStatic2(const MeshPrimData& data) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//�T���v���[�̏���
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		//���_�V�F�[�_
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�s�N�Z���V�F�[�_
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�ʏ���
		SimpleConstants SmCb;
		//�R���X�^���g�o�b�t�@�̍쐬
		SetConstants(SmCb, data);
		//�e�N�X�`��
		auto shTex = GetTextureResource();
		if (shTex) {
			//�e�N�X�`��������
			SmCb.ActiveFlg.x = 1;
		}
		else {
			//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
			if (shTex = data.m_TextureResource.lock()) {
				//�e�N�X�`��������
				SmCb.ActiveFlg.x = 1;
			}
			else {
				SmCb.ActiveFlg.x = 0;
			}
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = data.m_NumStride;
		UINT offset = 0;
		//�`����@�̃Z�b�g
		pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
		//pD3D11DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		//pD3D11DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//�e�����_�����O�X�e�[�g�̐ݒ�
		//�u�����h�X�e�[�g
		RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
		//�f�v�X�X�e���V���X�e�[�g
		RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
		//�e�N�X�`���ƃT���v���[
		if (shTex) {
			pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[��ݒ�
			RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
		}
		else {
			//�V�F�[�_�[���\�[�X���N���A
			pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pD3D11DeviceContext);
		}
		//�e�ƃT���v���[
		if (IsOwnShadowActive()) {
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//�V���h�E�}�b�v�T���v���[
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//���X�^���C�U�X�e�[�g�ƕ`��
		if (GetRasterizerState() == RasterizerState::DoubleDraw) {
			//���������p
			//���X�^���C�U�X�e�[�g(���`��)
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//�`��
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			//���X�^���C�U�X�e�[�g�i�\�`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//�`��
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		}
		else {
			RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
			//�`��
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		}
	}

}