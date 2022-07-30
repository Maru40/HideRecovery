/*!
@file ToonPNTStaticModelDraw.cpp
@brief ToonPNTStaticModelDraw�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "ToonPNTStaticModelDraw.h"

namespace basecross {

	//�R���X�^���g�o�b�t�@�ɓn�������p�����[�^�̂݋L�q����
	struct ToonPNTStaticModelDraw::Impl : public DrawObjectBase {
		//���_�ύX����ꍇ�̃��b�V���i�I���W�i���j
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//�I���W�i�����b�V�����g�����ǂ���
		bool m_UseOriginalMeshResource;
		//�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		//NormalMap�e�N�X�`��
		weak_ptr<TextureResource> m_NormalMapTextureResource;
		vector<weak_ptr<TextureResource>> m_NormalMapTextureResourceVec;
		//�G�~�b�V�u�F
		Col4 m_Emissive;
		//�f�t���[�Y�F
		Col4 m_Diffuse;
		//�����x
		float m_Alpha;
		//�X�y�L�����[�ƃp���[�F
		Col4 m_SpecularColorAndPower;
		//���C�e�B���O���邩�ǂ���
		bool m_LightingEnabled;
		//�s�N�Z�����C�e�B���O�����邩�ǂ���
		bool m_PreferPerPixelLighting;
		//�@���Ƀo�C�A�X�������邩�ǂ���
		bool m_BiasedNormals;
		//�A���r�G���g
		Col4 m_AmbientLightColor;
		//���C�g��MAX
		static const int MaxDirectionalLights = 3;
		//���C�g���L�����ǂ���
		bool m_LightEnabled[MaxDirectionalLights];
		//���C�g�̌���
		Vec3 m_LightDirection[MaxDirectionalLights];
		//���C�g�̃f�t���[�Y�F
		Col4 m_LightDiffuseColor[MaxDirectionalLights];
		//���C�g�̃X�y�L�����[�F
		Col4 m_LightSpecularColor[MaxDirectionalLights];
		//�t�H�O���L�����ǂ���
		bool m_FogEnabled;
		//�t�H�O�̊J�n�ʒu
		float m_FogStart;
		//�t�H�O�̏I���ʒu
		float m_FogEnd;
		//�t�H�O�F
		Col4 m_FogColor;
		//�t�H�O�x�N�g��
		Vec3 m_FogVector;
		//�e�𓊉e���邩�ǂ���
		bool m_OwnShadowActive;
		//���f���ɓ����Ă���Diffuse���g�����ǂ���
		bool m_ModelDiffusePriority;
		//���f���ɓ����Ă���Emissive���g�����ǂ���
		bool m_ModelEmissivePriority;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_ModelTextureEnabled;

		ToonPNTStaticModelDraw::Impl() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Alpha(1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(false),
			m_PreferPerPixelLighting(false),
			m_BiasedNormals(false),
			m_AmbientLightColor(0, 0, 0, 0),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(true),
			m_ModelEmissivePriority(true),
			m_ModelTextureEnabled(true)
		{
			static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
			static const bsm::Vec3 defaultLightDirection = { 0, -1, 0 };
			m_SpecularColorAndPower = defaultSpecular;
			for (int i = 0; i < MaxDirectionalLights; i++)
			{
				m_LightDirection[i] = defaultLightDirection;
				m_LightDiffuseColor[i] = g_XMZero;
				m_LightSpecularColor[i] = g_XMZero;
			}
		}

	};

	//--------------------------------------------------------------------------------------
	/// �g�D�[�����f���`��R���|�[�l���g�{��
	//--------------------------------------------------------------------------------------

	ToonPNTStaticModelDraw::ToonPNTStaticModelDraw(const std::shared_ptr<GameObject>& objPtr)
		:BcBaseDraw(objPtr)
	{}

	void ToonPNTStaticModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//�}���`���C�g�̐ݒ�
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}

		SetRampTextureResource(L"Ramp_Rock_TX");
		SetSamplerState(SamplerState::LinearClamp);
	}

	void ToonPNTStaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}

		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawModel2(PtrMeshResource->GetMashData());
		}

		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawModel2(vec[i]);
				}
			}
		}

		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	void ToonPNTStaticModelDraw::DrawModel2(const MeshPrimData& data) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//�T���v���[�̏���
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		//���_�V�F�[�_
		pD3D11DeviceContext->VSSetShader(ToonVS::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(ToonVS::GetPtr()->GetInputLayout());
		//�s�N�Z���V�F�[�_
		pD3D11DeviceContext->PSSetShader(ToonPS::GetPtr()->GetShader(), nullptr, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = data.m_NumStride;
		UINT offset = 0;
		//�`����@�̃Z�b�g
		pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//�e�����_�����O�X�e�[�g�̐ݒ�
		//�u�����h�X�e�[�g
		RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
		//�f�v�X�X�e���V���X�e�[�g
		RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
		//�e�ƃT���v���[
		if (GetOwnShadowActive()) {
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//�V���h�E�}�b�v�T���v���[
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//���C�g��ݒ�
		SetLightingParamaters();
		BasicConstants BcCb;
		//�R���X�^���g�o�b�t�@�̍쐬
		SetConstants(BcCb, data);
		//���b�V�����̃}�e���A���̔z��
		auto& MatVec = data.m_MaterialExVec;
		size_t MatIndex = 0;
		for (auto& m : MatVec) {
			if (m.m_TextureResource) {
				//�e�N�X�`��������
				BcCb.activeFlg.y = 1;
			}
			else {
				//�e�N�X�`�����Ȃ�
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				bsm::Col4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			//RampTexutreTextrue
			auto rampTex = GetRampTextureResource();
			if (rampTex) {
				//pD3D11DeviceContext->VSSetShaderResources(1, 1, rampTex->GetShaderResourceView().GetAddressOf());
				pD3D11DeviceContext->PSSetShaderResources(1, 1, rampTex->GetShaderResourceView().GetAddressOf());

				//�T���v���[��ݒ�
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}

			if (BcCb.activeFlg.y) {
				//�V�F�[�_�Ƀe�N�X�`���̐ݒ肪����Ă���
				//�T���v���[�̐ݒ�
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
				//�e�N�X�`���̑I��
				if (IsModelTextureEnabled()) {
					//���f���̃e�N�X�`�����L��
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				}
				else {
					//���f���̃e�N�X�`��������
					auto shTex = GetTextureResource();
					if (shTex) {
						//�R���|�[�l���g�Ƀe�N�X�`��������
						pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
					}
					else {
						//�R���|�[�l���g�Ƀe�N�X�`�����Ȃ�
						BcCb.activeFlg.y = 0;
					}
				}
				if (BcCb.activeFlg.y) {
					//�@���}�b�v
					if (GetNormalMapTextureResourceVec().size() > MatIndex) {
						//�����̖@���}�b�v������
						auto& normalVec = GetNormalMapTextureResourceVec();
						auto normalShptr = normalVec[MatIndex].lock();
						if (GetOwnShadowActive()) {
							//�Q�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(2, 1, normalShptr->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
						}
						else {
							//1�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(1, 1, normalShptr->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
						}
					}
					else if (GetNormalMapTextureResource()) {
						//�����ł͂Ȃ��Ĉ�����@���}�b�v������
						if (GetOwnShadowActive()) {
							//�Q�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
						}
						else {
							//1�Ԗڂɓ����
							pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
							//�T���v���[��ݒ�
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
						}
					}
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
			}
			else {
				if (!rampTex) {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
			}

			//���X�^���C�U�X�e�[�g�ƕ`��
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//���������p
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			MatIndex++;
		}
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void ToonPNTStaticModelDraw::SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource) {
		m_rampTextureResource = textureResource;
	}

	void ToonPNTStaticModelDraw::SetRampTextureResource(const wstring& key) {
		SetRampTextureResource(App::GetApp()->GetResource<TextureResource>(key));
	}

	std::shared_ptr<TextureResource> ToonPNTStaticModelDraw::GetRampTextureResource() const {
		return m_rampTextureResource;
	}

}