
/*!
@file FieldMap.h
@brief FieldMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	class MapCursor;
	class I_TeamMember;

	//--------------------------------------------------------------------------------------
	/// �t�B�[���h�}�b�v�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct FieldMap_Parametor
	{
		maru::Rect rect;

		FieldMap_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// �t�B�[���h�}�b�v
	//--------------------------------------------------------------------------------------
	class FieldMap : public maru::SingletonComponent<FieldMap>
	{
	public:
		using Parametor = FieldMap_Parametor;

	private:
		Parametor m_param;								//�p�����[�^
		Builder::VertexPCTParametor m_builderParam;		//�}�b�v�e�N�X�`�������p�����[�^

		std::weak_ptr<SpriteObject> m_mapTexture;		//�}�b�v�e�N�X�`��
		std::weak_ptr<MapCursor> m_cursor;				//�J�[�\��

		std::weak_ptr<I_TeamMember> m_teamMember;

		std::weak_ptr<SpriteObject> m_mapBoxTexture;

	public:
		FieldMap(const std::shared_ptr<GameObject>& objPtr);
		FieldMap(const std::shared_ptr<GameObject>& objPtr, const Builder::VertexPCTParametor builderParam);

		void OnLateStart() override;

	private:
		void SettingDraw();
		void SettingBoxDraw();

	public:
		void SetMapDraw(const bool isDraw);

		bool GetMapDraw() const;

		bool IsMapDraw() const;

		std::shared_ptr<SpriteObject> GetMapTexture() const;

		std::shared_ptr<SpriteObject> GetMapBoxTexture() const;

		std::shared_ptr<MapCursor> GetMapCursor() const;

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

		/// <summary>
		/// �e�N�X�`���}�b�v�̃X�P�[����Ԃ��B
		/// </summary>
		Vec3 GetMapTextureScale() const { return m_builderParam.scale; };

		void SetTeamMember(const std::shared_ptr<I_TeamMember>& teamMember);

		std::shared_ptr<I_TeamMember> GetTeamMember() const;
	};

}