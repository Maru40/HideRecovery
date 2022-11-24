#pragma once
#include "stdafx.h"

namespace basecross {
	// 高度なメッシュ作成補助クラス
	class AdvMeshUtil {
		// 法線と影響を受けるスケールのデータ
		struct NormalAndUVData {
			Vec3 Normal;		// 法線
			Vec2 AffectedScale;	// 影響を受けるスケール
		};

		/**
		 * @brief ベクトル同士の掛け算
		 *
		 * @param vec1 ベクトル1
		 * @param vec2 ベクトル 2
		 * @return それぞれのxとyをかけた値
		 */
		static Vec2 VectorRate(const Vec2& vec1, const Vec2& vec2);
	public:
		/**
		 * @brief スケールに合わせたUVを持つCubeの生成
		 *
		 * @param textureSize テクスチャの大きさ
		 * @param scale オブジェクトの大きさ
		 * @param vertices 頂点データ
		 * @param indices インデックスデータ
		 */
		static void CreateCube(float textureSize, const Vec3& scale,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);

		/**
		 * @brief スケールに合わせたUVを持つ板ポリの生成（Z-Up）
		 *
		 * @param textureSize テクスチャの大きさ
		 * @param scale オブジェクトの大きさ
		 * @param vertices 頂点データ
		 * @param indices インデックスデータ
		 */
		static void CreateBoardPoly(float textureSize, const Vec2& scale,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);

		static void CreateBoardPoly(float textureSize, const Vec2& scale,
			vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);
	};
}