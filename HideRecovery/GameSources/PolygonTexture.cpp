/*!
@file PolygonTexture.cpp
@brief �|���\������
*/

#include "stdafx.h"
#include "Scene.h"
#include "PolygonTexture.h"

namespace basecross
{
	void PolygonTexture::OnCreate()
	{
		//�|���̒�`
		Vec3 vertexPos = m_Data.vertexPos;
		Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-vertexPos.x / 2, +vertexPos.y / 2 , vertexPos.z / 2), color, Vec2(0.0f, 0.0f)},
			{Vec3(+vertexPos.x / 2, +vertexPos.y / 2 , vertexPos.z / 2), color, Vec2(1.0f, 0.0f)},
			{Vec3(-vertexPos.x / 2, -vertexPos.y / 2 , vertexPos.z / 2), color, Vec2(0.0f, 1.0f)},
			{Vec3(+vertexPos.x / 2, -vertexPos.y / 2 , vertexPos.z / 2), color, Vec2(1.0f, 1.0f)}
		};

		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices); //�`��p�R���|�[�l���g�̐ݒ�
		drawComp->SetTextureResource(m_Data.texture); //�e�N�X�`���̐ݒ�

		// �`���Ԃ̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);

		// �g�����X�t�H�[���̐ݒ�
		transform->SetPosition(m_Data.position);
		Vec3 scale = Vec3(m_Data.scale.x, m_Data.scale.y, 1.0f);
		transform->SetScale(scale);
		SetDrawLayer(0);
	}
}