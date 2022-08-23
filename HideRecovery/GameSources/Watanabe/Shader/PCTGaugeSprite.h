/*!
@file   PCTGaugeSprite.h
@brief  ゲージ描画コンポーネント
*/

#pragma once
#include "stdafx.h"
#include "MySpriteBaseDraw.h"

namespace basecross {
	struct GaugeSpriteConstants {
		// ワールド行列
		Mat4x4 World;
		// エミッシブ色
		Col4 Emissive;
		// デフューズ色
		Col4 Diffuse;
		// 割合としきい値とフラグ
		Vec4 RatioAndThresholdEtc;
		// 最大値
		Vec4 Maximum;
		GaugeSpriteConstants() {
			memset(this, 0, sizeof(GaugeSpriteConstants));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPCTGaugeSprite, GaugeSpriteConstants)
	DECLARE_DX11_VERTEX_SHADER(VSPCTGaugeSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTGaugeSprite)

	class PCTGaugeSprite :public MySpriteBaseDraw {
		// 割合（0～1）
		float m_rate;
		// 最大値
		float m_maximum;
		// しきい値
		float m_threshold;
		// 割合に応じたゲージの色にするときにグラデーションテクスチャキー
		wstring m_gaugeGradientTexKey;
		// 背景ありか
		bool m_isBackground;
	public:
		explicit PCTGaugeSprite(const shared_ptr<GameObject>& gameObjectPtr);
		explicit PCTGaugeSprite(const shared_ptr<GameObject>& gameObjectPtr,
			vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices);

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
		 * @brief ゲージの最大値の設定
		 *
		 * @param max 最大値
		 */
		void SetMaximum(float max) {
			m_maximum = max;
		}

		/**
		 * @brief ゲージの色の設定
		 *
		 * @param color ゲージの色
		 */
		void SetGaugeColor(const Col4& color) {
			SetDiffuse(color);
		}

		/**
		 * @brief グラデーションテクスチャの設定
		 *
		 * @param key
		 */
		void SetGaugeGradientTexture(const wstring& key) {
			m_gaugeGradientTexKey = key;
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
		void SetConstants(GaugeSpriteConstants& spCb);

		/**
		 * @brief スプライトメッシュの描画
		 *
		 * @param data メッシュのデータ
		 */
		void DrawSprite(const MeshPrimData& data);
	};
}
