/*!
@file SpriteObject.h
@brief SpriteObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "HitStopManager.h"

#include "ReactiveBool.h"
#include "BuilderVertexPCT.h"
#include "Maruyama/UI/UILayer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// スプライトオブジェクト
	//--------------------------------------------------------------------------------------
	class SpriteObject : public GameObject
	{
	public:
		using DrawComp = PCTSpriteDraw;
		
	private:
		std::shared_ptr<Builder::VertexPCTParametor> m_paramPtr;  //スプライト生成用パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="vertexParam">スプライト生成用のパラメータ</param>
		SpriteObject(const std::shared_ptr<Stage>& stage, const Builder::VertexPCTParametor& vertexParam);

		void OnCreate() override;

	};

	//--------------------------------------------------------------------------------------
	/// スプライトデータ
	//--------------------------------------------------------------------------------------
	struct SpriteData
	{
		Vec3 offset;                            //オフセット位置
		Builder::VertexPCTParametor buildParam; //Sprite生成用のパラメータ

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="offset">オフセット</param>
		/// <param name="buildParam">Sprite生成用のパラメータ</param>
		SpriteData(
			const Vec2& offset,
			const Builder::VertexPCTParametor& buildParam
		);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="offset">オフセット</param>
		/// <param name="buildParam">Sprite生成用のパラメータ</param>
		SpriteData(
			const Vec3& offset,
			const Builder::VertexPCTParametor& buildParam
		);
	};

	//--------------------------------------------------------------------------------------------

}