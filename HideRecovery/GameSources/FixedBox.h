/*!
@file FixedBox.h
@brief FixedBoxなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	固定ボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public StageObjectBase
	{
	protected:
		vector<VertexPositionNormalTexture> m_vertices;  //頂点配列
		vector<uint16_t> m_indices;                      //インディセス

		Vec3 m_meshCreateScale = Vec3(0.0f);			 //メッシュ生成スケール
		
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="name">オブジェクト名</param>
		/// <param name="texture">テクスチャ名</param>
		FixedBox(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			:StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// メッシュの更新
		/// </summary>
		void UpdateMesh();
	};

}

//endbasecross