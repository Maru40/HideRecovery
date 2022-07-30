/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	IMPLEMENT_DX11_VERTEX_SHADER(SlimeVS, App::GetApp()->GetShadersPath() + L"SlimeVS.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(SlimePS, App::GetApp()->GetShadersPath() + L"SlimePS.cso")

	IMPLEMENT_DX11_VERTEX_SHADER(MarchingCubeVS, App::GetApp()->GetShadersPath() + L"MarchingCubeVS.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(MarchingCubePS, App::GetApp()->GetShadersPath() + L"MarchingCubePS.cso")

	IMPLEMENT_DX11_VERTEX_SHADER(ToonVS, App::GetApp()->GetShadersPath() + L"ToonVS.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(ToonPS, App::GetApp()->GetShadersPath() + L"ToonPS.cso")


	//CS�̎��̕�
	IMPLEMENT_DX11_COMPUTE_SHADER(CSMarchingCube, App::GetApp()->GetShadersPath() + L"CSMarchingCube.cso")

}
//end basecross


