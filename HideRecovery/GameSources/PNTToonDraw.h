/*!
@file PNTToonDraw.h
@brief PNTToonDraw
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	/// トゥーン描画コンポ―ネント
	//--------------------------------------------------------------------------------------
	class PNTToonDraw : public BcBaseDraw
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
		PNTToonDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="data">メッシュデータ</param>
		void DrawStatic2(const MeshPrimData& data);

		/// <summary>
		/// ランプテクスチャの設定
		/// </summary>
		/// <param name="textureResource">ランプテクスチャリソース</param>
		void SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource);

	public:
		/// <summary>
		/// ランプテクスチャの設定
		/// </summary>
		/// <param name="key">設定したいテクスチャ名</param>
		void SetRampTextureResource(const wstring& key);

		/// <summary>
		/// ランプテクスチャリソースの取得
		/// </summary>
		/// <returns>ランプテクスチャリソース</returns>
		std::shared_ptr<TextureResource> GetRampTextureResource() const;

	};

}