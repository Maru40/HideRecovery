#include "stdafx.h"
#include "MySpriteBaseDraw.h"

namespace basecross {
	MySpriteBaseDraw::MySpriteBaseDraw(const shared_ptr<GameObject>& gameObjectPtr) :
		DrawComponent(gameObjectPtr),
		m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
		m_emissive(0, 0, 0, 0)
	{}

	MySpriteBaseDraw::~MySpriteBaseDraw() {}

	shared_ptr<MeshResource> MySpriteBaseDraw::GetMeshResource() const {
		return m_spriteMesh;
	}
	void MySpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		m_spriteMesh = MeshRes;
	}

	void MySpriteBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		m_textureResource = TextureRes;
	}

	void MySpriteBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> MySpriteBaseDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = m_textureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	Col4 MySpriteBaseDraw::GetEmissive() const {
		return m_emissive;
	}
	void MySpriteBaseDraw::SetEmissive(const Col4& col) {
		m_emissive = col;
	}

	Col4 MySpriteBaseDraw::GetDiffuse() const {
		return m_diffuse;
	}
	void MySpriteBaseDraw::SetDiffuse(const Col4& col) {
		m_diffuse = col;
	}
}