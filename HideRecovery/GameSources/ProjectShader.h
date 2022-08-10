/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"
#include "Watanabe/Shader/AdvBaseDraw.h"

namespace basecross {
	DECLARE_DX11_VERTEX_SHADER(SlimeVS, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(SlimePS)

	DECLARE_DX11_VERTEX_SHADER(MarchingCubeVS, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(MarchingCubePS)

	DECLARE_DX11_VERTEX_SHADER(ToonVS, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(ToonPS)

	//�R���s���[�g�V�F�[�_
	DECLARE_DX11_COMPUTE_SHADER(CSMarchingCube)

	DECLARE_DX11_CONSTANT_BUFFER(CBAdvBaseDraw, AdvConstants)
	DECLARE_DX11_VERTEX_SHADER(VSBoneModelDraw, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSModelDraw, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSModelDraw)
}
//end basecross
