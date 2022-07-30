#pragma once
/*!
@file ToonPNTStaticModelDraw.h
@brief ToonPNTStaticModelDraw
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	/// トゥーンモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	class ToonPNTStaticModelDraw : public BcBaseDraw
	{
		//イディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

		std::shared_ptr<TextureResource> m_rampTextureResource = nullptr; //ランプテクスチャリソース

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ToonPNTStaticModelDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// モデルのドロー
		/// </summary>
		/// <param name="data">メッシュデータ</param>
		void DrawModel2(const MeshPrimData& data);

		/// <summary>
		/// ランプテクスチャリソースの設定
		/// </summary>
		/// <param name="textureResource">テクスチャリソース</param>
		void SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource);

	public:
		/// <summary>
		/// ランプテクスチャリソースの設定
		/// </summary>
		/// <param name="key">テクスチャ名</param>
		void SetRampTextureResource(const wstring& key);

		/// <summary>
		/// テクスチャリソースの取得
		/// </summary>
		/// <returns>テクスチャリソース</returns>
		std::shared_ptr<TextureResource> GetRampTextureResource() const;

	};

}