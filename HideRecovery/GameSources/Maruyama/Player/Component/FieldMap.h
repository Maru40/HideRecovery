
/*!
@file FieldMap.h
@brief FieldMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BuilderVertexPCT.h"
#include "SingletonComponent.h"

#include "ImpactMap.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SpriteObject;

	//--------------------------------------------------------------------------------------
	/// フィールドマップのパラメータ
	//--------------------------------------------------------------------------------------
	struct FieldMap_Parametor
	{
		maru::Rect rect;

		FieldMap_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// フィールドマップ
	//--------------------------------------------------------------------------------------
	class FieldMap : public maru::SingletonComponent<FieldMap>
	{
	public:
		using Parametor = FieldMap_Parametor;

	private:
		Parametor m_param;								//パラメータ
		Builder::VertexPCTParametor m_builderParam;		//マップテクスチャ生成パラメータ

		std::weak_ptr<SpriteObject> m_mapTexture;		//マップテクスチャ

	public:
		FieldMap(const std::shared_ptr<GameObject>& objPtr);
		FieldMap(const std::shared_ptr<GameObject>& objPtr, const Builder::VertexPCTParametor builderParam);

		void OnLateStart() override;

	private:
		void SettingDraw();

	public:
		void SetMapDraw(const bool isDraw);

		bool GetMapDraw() const;

		std::shared_ptr<SpriteObject> GetMapTexture() const;

		void SetFieldSize(const std::shared_ptr<GameObject>& floor) {
			auto scale = floor->GetComponent<Transform>()->GetScale();
			SetFieldSize(scale.x, scale.z);
		}

		void SetFieldSize(const float width, const float depth) { 
			m_param.rect.width = width;
			m_param.rect.depth = depth;
		}

		float GetWidth() const noexcept { return m_param.rect.width; }

		float GetDepth() const noexcept { return m_param.rect.depth; }

		maru::Rect GetRect() const { return m_param.rect; }
	};

}