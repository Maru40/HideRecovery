
/*!
@file FieldMap.cpp
@brief FieldMapクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FieldMap.h"

#include "SpriteObject.h"

namespace basecross {

	std::weak_ptr<FieldMap>  maru::SingletonComponent<FieldMap>::sm_instance;

	//--------------------------------------------------------------------------------------
	/// フィールドマップのパラメータ
	//--------------------------------------------------------------------------------------

	FieldMap_Parametor::FieldMap_Parametor() :
		rect(maru::Rect(Vec3(0.0f), 100.0f, 200.0f))
		//width(100.0f), 
		//depth(200.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// フィールドマップ本体
	//--------------------------------------------------------------------------------------

	FieldMap::FieldMap(const std::shared_ptr<GameObject>& objPtr) :
		FieldMap(objPtr, Builder::VertexPCTParametor(Vec3(256.0f, 512.0f, 0.0f) * 2.0f, Vec2(256.0f, 512.0f), L"FiledMap_TX"))
	{}

	FieldMap::FieldMap(const std::shared_ptr<GameObject>& objPtr, const Builder::VertexPCTParametor builderParam) :
		SingletonComponent(objPtr),
		m_param(Parametor()),
		m_builderParam(builderParam)
	{}

	void FieldMap::OnLateStart() {
		SettingDraw();
	}

	void FieldMap::SettingDraw() {
		auto builder = Builder::BuilderVertexPCT(m_builderParam);

		auto mapTextrue = GetStage()->AddGameObject<SpriteObject>(m_builderParam);

		m_mapTexture = mapTextrue;
	}

	void FieldMap::SetMapDraw(const bool isDraw) {
		GetMapTexture()->SetDrawActive(isDraw);
	}

	bool FieldMap::GetMapDraw() const { return GetMapTexture()->GetDrawActive(); }

	std::shared_ptr<SpriteObject> FieldMap::GetMapTexture() const {
		return m_mapTexture.lock();
	}

}