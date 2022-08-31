/*!
@file AuraBase.h
@brief AuraBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BuilderVertexPCT.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// オーラ更新パラメータ
		//--------------------------------------------------------------------------------------
		struct Aura_UpdateParametor {
			float speed;  //移動スピード
			Vec2 direct;  //移動方向

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Aura_UpdateParametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">移動スピード</param>
			/// <param name="direct">移動方向</param>
			Aura_UpdateParametor(const float& speed, const Vec2& direct);
		};

		//--------------------------------------------------------------------------------------
		/// オーラパラメータ
		//--------------------------------------------------------------------------------------
		struct Aura_Parametor
		{
			int sides;                         //サイドス  
			float height;                      //高さ
			float topRadius;                   //上の半径
			float bottomRadius;                //下の半径
			Col4 topColor;                     //上のカラー
			Col4 bottomColor;                  //下のカラー
			wstring texture;                   //テクスチャ名
			Aura_UpdateParametor updateParam;  //更新用パラメータ
			BlendState blendStage;             //ブレンドステート

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Aura_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="sides">サイドス</param>
			/// <param name="height">高さ</param>
			/// <param name="topRadius">上の半径</param>
			/// <param name="bottomRadius">下の半径</param>
			/// <param name="color">カラー</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="updateParam">更新パラメータ</param>
			Aura_Parametor(
				const int& sides, const float& height,
				const float& topRadius, const float& bottomRadius,
				const Col4& color, 
				const wstring& texture,
				const Aura_UpdateParametor& updateParam
			);

		};

		//--------------------------------------------------------------------------------------
		/// オーラの基底クラス
		//--------------------------------------------------------------------------------------
		class AuraBase : public Component
		{
		public:
			using Parametor = Aura_Parametor;
			using UpdateParametor = Aura_UpdateParametor;
			using DrawComp = BcPCTStaticDraw;

		private:
			Parametor m_param;                                   //パラメータ
			std::vector<VertexPositionColorTexture> m_vertices;  //頂点配列

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			AuraBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			AuraBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// UVアップデート
			/// </summary>
			void UpdateUV();

			/// <summary>
			/// 頂点の生成
			/// </summary>
			/// <returns>頂点配列</returns>
			std::vector<VertexPositionColorTexture> CreateVertices();

			/// <summary>
			/// インディセスの生成
			/// </summary>
			/// <returns>インディセス</returns>
			std::vector<uint16_t> CreateIndices();

		};

	}
}