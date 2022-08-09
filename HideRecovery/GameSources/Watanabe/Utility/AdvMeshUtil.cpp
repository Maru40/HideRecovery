#include "stdafx.h"
#include "AdvMeshUtil.h"

namespace basecross {
	void AdvMeshUtil::CreateCube(float textureSize, const Vec3& scale,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			// テクスチャの大きさに応じたUVの値
			const float uvRate = 1 / textureSize;
			// テクスチャUV
			const Vec2 textureCoordinates[4] =
			{
				{ uvRate, 0      },
				{ uvRate, uvRate },
				{ 0     , uvRate },
				{ 0     , 0      },
			};

			// 法線と影響を受けるサイズの軸
			const vector<NormalAndUVData> data = {
				{{ 0 ,  0,  1 },Vec2(scale.x,scale.y)},
				{{ 0 ,  0, -1 },Vec2(scale.x,scale.y)},
				{{ 1 ,  0,  0 },Vec2(scale.z,scale.y)},
				{{ -1,  0,  0 },Vec2(scale.z,scale.y)},
				{{ 0 ,  1,  0 },Vec2(scale.x,scale.z)},
				{{ 0 , -1,  0 },Vec2(scale.x,scale.z)},
			};

			// 一辺の半分の長さ
			const float size = 0.5f;
			//Face数は6
			const int FaceCount = 6;
			for (int i = 0; i < FaceCount; i++)
			{
				//法線
				XMVECTOR normal = data[i].Normal;

				XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

				XMVECTOR side1 = XMVector3Cross(normal, basis);
				XMVECTOR side2 = XMVector3Cross(normal, side1);

				//インデックスの登録
				size_t vbase = vertices.size();
				indices.push_back((uint16_t)vbase + 0);
				indices.push_back((uint16_t)vbase + 1);
				indices.push_back((uint16_t)vbase + 2);

				indices.push_back((uint16_t)vbase + 0);
				indices.push_back((uint16_t)vbase + 2);
				indices.push_back((uint16_t)vbase + 3);

				vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, VectorRate(data[i].AffectedScale, textureCoordinates[0])));
				vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, VectorRate(data[i].AffectedScale, textureCoordinates[1])));
				vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, VectorRate(data[i].AffectedScale, textureCoordinates[2])));
				vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, VectorRate(data[i].AffectedScale, textureCoordinates[3])));
			}
			//RHからLHに変更
			MeshUtill::ReverseWinding(indices, vertices);
		}
		catch (...) {
			throw;
		}
	}

	Vec2 AdvMeshUtil::VectorRate(const Vec2& vec1, const Vec2& vec2) {
		return Vec2(vec1.x * vec2.x, vec1.y * vec2.y);
	}
}