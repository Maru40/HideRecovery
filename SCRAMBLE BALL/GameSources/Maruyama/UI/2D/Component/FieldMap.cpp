
/*!
@file FieldMap.cpp
@brief FieldMap�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FieldMap.h"

#include "Maruyama/UI/2D/Object/SpriteObject.h"
#include "MapCursor.h"
#include "Maruyama/UI/2D/UILayer.h"

#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {

	std::weak_ptr<FieldMap>  maru::SingletonComponent<FieldMap>::sm_instance;

	//--------------------------------------------------------------------------------------
	/// �t�B�[���h�}�b�v�̃p�����[�^
	//--------------------------------------------------------------------------------------

	FieldMap_Parametor::FieldMap_Parametor() :
		rect(maru::Rect(Vec3(0.0f), 100.0f, 200.0f))
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
		SettingBoxDraw();
	}

	void FieldMap::SettingDraw() {
		//�}�b�v�e�N�X�`���̐���
		//auto builder = Builder::BuilderVertexPCT(m_builderParam);
		auto mapTexture = GetStage()->AddGameObject<SpriteObject>(m_builderParam);
		mapTexture->SetDrawLayer(UI::Layer::MAP);
		m_mapTexture = mapTexture;

		//�J�[�\������
		auto cursor = GetStage()->AddGameObject<GameObject>()->AddComponent<MapCursor>();
		m_cursor = cursor;
	}

	void FieldMap::SettingBoxDraw() {
		//�}�b�v�e�N�X�`���̐���
		auto param = Builder::VertexPCTParametor(Vec3(256.0f, 512.0f, 0.0f) * 2.0f, Vec2(512.0f, 1024.0f), L"FieldMapBox_TX");
		//auto builder = Builder::BuilderVertexPCT(m_builderParam);
		auto mapTexture = GetStage()->AddGameObject<SpriteObject>(param);
		mapTexture->SetDrawLayer(UI::Layer::MAP_BOX);
		m_mapBoxTexture = mapTexture;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void FieldMap::SetMapDraw(const bool isDraw) {
		auto teamMember = GetMapCursor()->GetTarget()->GetComponent<I_TeamMember>(false);
		if (!teamMember) {
			return;
		}

		if (teamMember->GetTeam() == team::TeamType::Red) {
			GetMapTexture()->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 0.0f, XM_PI));
			GetMapBoxTexture()->GetComponent<Transform>()->SetScale(Vec3(-1.0f, 1.0f, 1.0f));
		}
		else {
			GetMapTexture()->GetComponent<Transform>()->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
			GetMapBoxTexture()->GetComponent<Transform>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		}

		GetMapBoxTexture()->SetDrawActive(isDraw);
		GetMapTexture()->SetDrawActive(isDraw);
		GetMapCursor()->SetDrawActive(isDraw);
	}

	bool FieldMap::GetMapDraw() const { return GetMapTexture()->GetDrawActive(); }

	bool FieldMap::IsMapDraw() const { return GetMapDraw(); }

	std::shared_ptr<SpriteObject> FieldMap::GetMapTexture() const { return m_mapTexture.lock(); }

	std::shared_ptr<SpriteObject> FieldMap::GetMapBoxTexture() const { return m_mapBoxTexture.lock(); }

	std::shared_ptr<MapCursor> FieldMap::GetMapCursor() const { return m_cursor.lock(); }

	void FieldMap::SetTeamMember(const std::shared_ptr<I_TeamMember>& teamMember) {
		m_teamMember = teamMember;
	}

	std::shared_ptr<I_TeamMember> FieldMap::GetTeamMember() const {
		return m_teamMember.lock();
	}

}