
/*!
@file FieldMap.cpp
@brief FieldMap�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FieldMap.h"

#include "SpriteObject.h"
#include "MapCursor.h"

namespace basecross {

	std::weak_ptr<FieldMap>  maru::SingletonComponent<FieldMap>::sm_instance;

	//--------------------------------------------------------------------------------------
	/// �t�B�[���h�}�b�v�̃p�����[�^
	//--------------------------------------------------------------------------------------

	FieldMap_Parametor::FieldMap_Parametor() :
		rect(maru::Rect(Vec3(0.0f), 100.0f, 200.0f))
		//width(100.0f), 
		//depth(200.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// �t�B�[���h�}�b�v�{��
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
		//�}�b�v�e�N�X�`���̐���
		auto builder = Builder::BuilderVertexPCT(m_builderParam);
		auto mapTextrue = GetStage()->AddGameObject<SpriteObject>(m_builderParam);
		m_mapTexture = mapTextrue;

		//�J�[�\������
		auto cursor = GetStage()->AddGameObject<GameObject>()->AddComponent<MapCursor>();
		m_cursor = cursor;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void FieldMap::SetMapDraw(const bool isDraw) {
		GetMapTexture()->SetDrawActive(isDraw);
		GetMapCursor()->SetDrawActive(isDraw);
	}

	bool FieldMap::GetMapDraw() const { return GetMapTexture()->GetDrawActive(); }

	std::shared_ptr<SpriteObject> FieldMap::GetMapTexture() const { return m_mapTexture.lock(); }

	std::shared_ptr<MapCursor> FieldMap::GetMapCursor() const { return m_cursor.lock(); }

}