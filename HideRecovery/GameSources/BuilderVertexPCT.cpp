/*!
@file BuilderVertexPCT.cpp
@brief BuilderVertexPCT����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "BuilderVertexPCT.h"

namespace basecross {
	namespace Builder {

		//--------------------------------------------------------------------------------------
		///	VertexPCTParametor
		//--------------------------------------------------------------------------------------

		VertexPCTParametor::VertexPCTParametor(const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			VertexPCTParametor(
				Vec3(1.0f, 1.0f, 0.0f),
				useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale,
			const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			VertexPCTParametor(
				scale, Col4(1.0f),
				useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale, const float& size,
			const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			VertexPCTParametor(
				scale, size, Col4(1.0f),
				useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale, const Col4& color,
			const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			VertexPCTParametor(
				scale, 1.0f, color,
				useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
			const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		) :
			VertexPCTParametor(
				scale, size, color,
				Vec2(0.0f, useSpriteSize.x), Vec2(0.0f, useSpriteSize.y), useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale, const Col4& color,
			const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			VertexPCTParametor(
				scale, 1.0f, color,
				useUVRange_X, useUVRange_Y, useSpriteSize,
				texture, pivot
			)
		{}

		VertexPCTParametor::VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
			const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
			const wstring& texture, const UIPivot pivot
		):
			scale(scale), size(size), color(color),
			useUVRange_X(useUVRange_X), useUVRange_Y(useUVRange_Y), useSpriteSize(useSpriteSize),
			texture(texture), pivot(pivot)
		{}

		//--------------------------------------------------------------------------------------
		///	�X�v���C�g�����N���X�{��
		//--------------------------------------------------------------------------------------

		BuilderVertexPCT::BuilderVertexPCT(const VertexPCTParametor& parametor)
		{
			CreateVertices(parametor);  //���_�f�[�^�̐���

			m_indices = {  //�C���f�B�Z�X�̐���
				0, 1, 2,
				2, 1, 3
			};
		}

		vector<Vec2> BuilderVertexPCT::CreateUseUV(const VertexPCTParametor& parametor) {
			const auto& xRange = parametor.useUVRange_X;
			const auto& yRange = parametor.useUVRange_Y;
			const Vec2& size = parametor.useSpriteSize;

			const vector<Vec2> useUV = {
				{xRange.start / size.x , yRange.start / size.y},
				{xRange.end / size.x , yRange.start / size.y},
				{xRange.start / size.x , yRange.end / size.y},
				{xRange.end / size.x , yRange.end / size.y}
			};

			return useUV;
		}

		void BuilderVertexPCT::CreateVertices(const VertexPCTParametor& parametor) {
			const float ComvertHalfSize = 0.5f;
			float width = parametor.scale.x * ComvertHalfSize;
			float height = parametor.scale.y * ComvertHalfSize;
			float depth = parametor.scale.z * ComvertHalfSize;
			const float& size = parametor.size;

			const Col4& color = parametor.color;

			const auto& pivot = parametor.pivot;

			auto useUV = CreateUseUV(parametor);

			switch (pivot)
			{
			case UIPivot::Center: //���S
				m_vertices = {
					{Vec3(-width,+height,+depth) * size, color, useUV[0]},
					{Vec3(+width,+height,+depth) * size, color, useUV[1]},
					{Vec3(-width,-height,-depth) * size, color, useUV[2]},
					{Vec3(+width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Up:  //���
				height *= 2.0f;

				m_vertices = {
					{Vec3(-width,+0.0f,+depth) * size, color, useUV[0]},
					{Vec3(+width,+0.0f,+depth) * size, color, useUV[1]},
					{Vec3(-width,-height,-depth) * size, color, useUV[2]},
					{Vec3(+width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Down:  //����
				height *= 2.0f;

				m_vertices = {
					{Vec3(-width,+height,+depth) * size, color, useUV[0]},
					{Vec3(+width,+height,+depth) * size, color, useUV[1]},
					{Vec3(-width,+0.0f,-depth) * size, color, useUV[2]},
					{Vec3(+width,+0.0f,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Left:  //����
				//���S�ɂ����ꍇ������width�ōςނ���A
				//�t�ɍ��悹�œ����傫������邽�߂ɂ́A��{�ɂ��Ȃ���΂Ȃ�Ȃ�
				width *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+height,+depth) * size, color, useUV[0]},
					{Vec3(+width,+height,+depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,-height,-depth) * size, color, useUV[2]},
					{Vec3(+width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::LeftUp:  //�����

				width *= 2.0f;
				height *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+0.0f  ,+depth) * size, color, useUV[0]},
					{Vec3(+width,+0.0f  ,+depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,-height,-depth) * size, color, useUV[2]},
					{Vec3(+width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::LeftDown:  //������

				width *= 2.0f;
				height *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+height  ,+depth) * size, color, useUV[0]},
					{Vec3(+width,+height  ,+depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,+0.0f,    -depth) * size, color, useUV[2]},
					{Vec3(+width,+0.0f,    -depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Right:  //�E��

				width *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+height,+depth) * size, color, useUV[0]},
					{Vec3(-width,+height,+depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,-height,-depth) * size, color, useUV[2]},
					{Vec3(-width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::RightUp:  //�E���

				width *= 2.0f;
				height *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+0.0f,  +depth) * size, color, useUV[0]},
					{Vec3(-width,+0.0f,  +depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,-height,-depth) * size, color, useUV[2]},
					{Vec3(-width,-height,-depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::RightDown:  //�E����

				width *= 2.0f;
				height *= 2.0f;

				m_vertices = {
					{Vec3(+0.0f ,+height,+depth) * size, color, useUV[0]},
					{Vec3(-width,+height,+depth) * size, color, useUV[1]},
					{Vec3(+0.0f ,+0.0f,  -depth) * size, color, useUV[2]},
					{Vec3(-width,+0.0f,  -depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Forward:  //�O��

				depth *= 2.0f;

				m_vertices = {
					{Vec3(+width ,+height, +0.0f ) * size, color, useUV[0]},
					{Vec3(-width ,+height, +0.0f ) * size, color, useUV[1]},
					{Vec3(+width ,-height, -depth) * size, color, useUV[2]},
					{Vec3(-width ,-height, -depth) * size, color, useUV[3]},
				};

				break;

			case UIPivot::Back:  //���

				depth *= 2.0f;

				m_vertices = {
					{Vec3(+width ,+height, +0.0f) * size, color, useUV[0]},
					{Vec3(-width ,+height, +0.0f) * size, color, useUV[1]},
					{Vec3(+width ,-height, +depth) * size, color, useUV[2]},
					{Vec3(-width ,-height, +depth) * size, color, useUV[3]},
				};

				break;
			}
		}

	}
}

//endbasecross
