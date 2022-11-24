/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	IMPLEMENT_DX11_VERTEX_SHADER(SlimeVS, App::GetApp()->GetShadersPath() + L"SlimeVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(SlimePS, App::GetApp()->GetShadersPath() + L"SlimePS.cso")

		IMPLEMENT_DX11_VERTEX_SHADER(MarchingCubeVS, App::GetApp()->GetShadersPath() + L"MarchingCubeVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(MarchingCubePS, App::GetApp()->GetShadersPath() + L"MarchingCubePS.cso")

		IMPLEMENT_DX11_VERTEX_SHADER(ToonVS, App::GetApp()->GetShadersPath() + L"ToonVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(ToonPS, App::GetApp()->GetShadersPath() + L"ToonPS.cso")

		//CS�̎��̕�
		IMPLEMENT_DX11_COMPUTE_SHADER(CSMarchingCube, App::GetApp()->GetShadersPath() + L"CSMarchingCube.cso")

		IMPLEMENT_DX11_CONSTANT_BUFFER(CBAdvBaseDraw)
		IMPLEMENT_DX11_CONSTANT_BUFFER(CBOutlineDraw)
		IMPLEMENT_DX11_VERTEX_SHADER(VSBoneModelDraw, App::GetApp()->GetShadersPath() + L"VSBoneModelDraw.cso")
		IMPLEMENT_DX11_VERTEX_SHADER(VSBoneModelDrawShadow, App::GetApp()->GetShadersPath() + L"VSBoneModelDrawShadow.cso")
		IMPLEMENT_DX11_VERTEX_SHADER(VSModelDraw, App::GetApp()->GetShadersPath() + L"VSModelDraw.cso")
		IMPLEMENT_DX11_VERTEX_SHADER(VSModelDrawShadow, App::GetApp()->GetShadersPath() + L"VSModelDrawShadow.cso")

		IMPLEMENT_DX11_VERTEX_SHADER(VSOutlineDraw, App::GetApp()->GetShadersPath() + L"VSOutlineDraw.cso")
		IMPLEMENT_DX11_VERTEX_SHADER(VSBoneOutlineDraw, App::GetApp()->GetShadersPath() + L"VSBoneOutlineDraw.cso")

		IMPLEMENT_DX11_PIXEL_SHADER(PSModelDraw, App::GetApp()->GetShadersPath() + L"PSModelDraw.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSModelDrawShadow, App::GetApp()->GetShadersPath() + L"PSModelDrawShadow.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSModelDrawShadow2, App::GetApp()->GetShadersPath() + L"PSModelDrawShadow2.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSOutlineDraw, App::GetApp()->GetShadersPath() + L"PSOutlineDraw.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSBarrierDraw, App::GetApp()->GetShadersPath() + L"PSBarrierDraw.cso")
}
//end basecross