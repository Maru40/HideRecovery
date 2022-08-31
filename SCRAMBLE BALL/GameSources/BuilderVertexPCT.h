/*!
@file BuilderVertexPCT.h
@brief BuilderVertexPCT
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Builder
	{
		//--------------------------------------------------------------------------------------
		///	UIの軸
		//--------------------------------------------------------------------------------------
		enum class UIPivot {
			Center,		//中心
			Up,			//上
			Down,		//下
			Left,		//左
			LeftUp,		//左上
			LeftDown,	//左下
			Right,		//右
			RightUp,	//右上
			RightDown,	//右下
			Forward,	//正面
			Back		//後方
		};

		//--------------------------------------------------------------------------------------
		///	使用するUV範囲
		//--------------------------------------------------------------------------------------
		struct UseUVRange
		{
			float start;  //開始位置
			float end;    //終了位置

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="start">開始位置</param>
			/// <param name="end">終了位置</param>
			UseUVRange(const float start, const float end)
				:start(start), end(end)
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="range">範囲</param>
			UseUVRange(const Vec2& range)
				:start(range.x), end(range.y)
			{}
		};

		//--------------------------------------------------------------------------------------
		///	スプライト生成パラメータ
		//--------------------------------------------------------------------------------------
		struct VertexPCTParametor
		{
			Vec3 scale;               //スケール
			float size;               //全体サイズ

			Col4 color;               //色

			UseUVRange useUVRange_X;  //使用するUV範囲のX
			UseUVRange useUVRange_Y;  //使用するUV範囲のY

			Vec2 useSpriteSize;       //画像の大きさ
			wstring texture;          //テクスチャ
			UIPivot pivot;            //ピボット

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, 
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="size">全体サイズ</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, const float& size,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="color">色</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, const Col4& color,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="size">全体サイズ</param>
			/// <param name="color">色</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="color">色</param>
			/// <param name="useUVRange_X">使用するUV範囲のX</param>
			/// <param name="useUVRange_Y">使用するUV範囲のY</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, const Col4& color,
				const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="scale">大きさ</param>
			/// <param name="size">全体サイズ</param>
			/// <param name="color">色</param>
			/// <param name="useUVRange_X">使用するUV範囲のX</param>
			/// <param name="useUVRange_Y">使用するUV範囲のY</param>
			/// <param name="useSpriteSize">画像の大きさ</param>
			/// <param name="texture">テクスチャ名</param>
			/// <param name="pivot">軸</param>
			VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
				const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);
		};

		//--------------------------------------------------------------------------------------
		///	スプライト生成クラス
		//--------------------------------------------------------------------------------------
		class BuilderVertexPCT
		{
		public:

			vector<VertexPositionColorTexture> m_vertices;  //頂点配列
			vector<uint16_t> m_indices;						//インデックス配列

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="parametor">スプライト生成用パラメータ</param>
			BuilderVertexPCT(const VertexPCTParametor& parametor);

		private:
			/// <summary>
			/// 使用するUV範囲を生成
			/// </summary>
			/// <param name="parametor">生成用パラメータ</param>
			/// <returns>使用するUV範囲</returns>
			vector<Vec2> CreateUseUV(const VertexPCTParametor& parametor);

			/// <summary>
			/// 頂点データを生成する。
			/// </summary>
			/// <param name="parametor">生成用パラメータ</param>
			void CreateVertices(const VertexPCTParametor& parametor);

		};

	}
}

//endbasecross