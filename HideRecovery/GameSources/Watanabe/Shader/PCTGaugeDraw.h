#pragma once
#include "stdafx.h"
#include "AdvBaseDraw.h"

namespace basecross {
	struct GaugeDrawConstants {
		// ワールド行列
		Mat4x4 World;
		// ビュー行列
		Mat4x4 View;
		// 射影行列
		Mat4x4 Projection;
		// エミッシブ色
		Col4 Emissive;
		// デフューズ色
		Col4 Diffuse;
		// 割合としきい値とフラグ
		Vec4 RatioAndThresholdEtc;

		GaugeDrawConstants() {
			memset(this, 0, sizeof(GaugeDrawConstants));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPCTGaugeDraw, GaugeDrawConstants)
	DECLARE_DX11_VERTEX_SHADER(VSPCTGaugeDraw, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTGaugeDraw)

	class PCTGaugeDraw :public DrawComponent {
		// 割合（0～1）
		float m_rate;
		// しきい値
		float m_threshold;
		// 背景ありか
		bool m_isBackground;
		// メッシュ
		shared_ptr<MeshResource> m_meshResource;
		// テクスチャリソース
		weak_ptr<TextureResource> m_textureResource;
		// エミッシブ色
		Col4 m_emissive;
		// デフューズ色
		Col4 m_diffuse;
	public:
		explicit PCTGaugeDraw(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override {}
		void OnDraw()override;

		/**
		 * @brief ゲージ残量の設定
		 *
		 * @param rate 割合
		 */
		void SetRate(float rate) {
			m_rate = rate;
		}

		/**
		 * @brief ゲージの色の設定
		 *
		 * @param color ゲージの色
		 */
		void SetGaugeColor(const Col4& color) {
			m_diffuse = color;
		}

		/**
		 * @brief 背景をつけるか
		 *
		 * @param flg trueならあり
		 */
		void IsBackground(bool flg) {
			m_isBackground = flg;
		}
	private:
		/**
		 * @brief コンスタントバッファの設定
		 *
		 * @param spCb 設定するコンスタントバッファ構造体
		 */
		void SetConstants(GaugeDrawConstants& spCb);

		/**
		 * @brief スプライトメッシュの描画
		 *
		 * @param data メッシュのデータ
		 */
		void DrawMesh(const MeshPrimData& data);
	};
}
