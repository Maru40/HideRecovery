
/*!
@file Reticle.cpp
@brief Reticle�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Reticle.h"

#include "Maruyama/UI/2D/Object/SpriteObject.h"
#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	Reticle::Reticle(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void Reticle::OnCreate() {
		//Reticle_TX
		//ReticleCnterRed_TX
		auto param = Builder::VertexPCTParametor(Vec3(64.0f, 64.0f, 0.0f), 1.0f, Vec2(512.0f, 512.0f), L"ReticleCnterRed_TX");
		auto sprite = GetStage()->AddGameObject<SpriteObject>(param);
		sprite->SetAlphaActive(true);

		auto spriteTrans = sprite->GetComponent<Transform>();
		Vec3 position = Vec3(0.0f, 100.0f, 0.0f);
		spriteTrans->SetPosition(position);
		
		sprite->GetComponent<SpriteObject::DrawComp>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.5f));

		m_sprite = sprite;
	}

}