/*!
@file   BoneModelDraw.h
@brief  ボーン付きのモデル描画コンポーネント
*/

#pragma once
#include "stdafx.h"
#include "ProjectShader.h"

namespace basecross {
	class BoneModelDraw : public SmBaseDraw {
	public:
		explicit BoneModelDraw(const shared_ptr<GameObject>& owner);
		virtual ~BoneModelDraw();

		/// <summary>
		/// メッシュリソースの設定
		/// </summary>
		/// <param name="meshRes">メッシュリソース</param>
		virtual void SetMeshResource(const shared_ptr<MeshResource>& meshRes)override;

		/// <summary>
		/// メッシュリソースの登録
		/// </summary>
		/// <param name="meshKey">登録されているメッシュキー</param>
		void SetMeshResource(const wstring& meshKey);

		/// <summary>
		/// マルチメッシュリソースを設定する
		/// </summary>
		/// <param name="meshRes">メッシュリソース</param>
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& meshRes)override;

		/// <summary>
		/// マルチメッシュリソースを設定する
		/// </summary>
		/// <param name="meshKey">メッシュリソースのキー</param>
		void SetMultiMeshResource(const wstring& meshKey);

		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	};
}
