/*!
@file CircleMarkerObject.cpp
@brief CircleMarkerObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CircleMarkerObject.h"

#include "BuilderVertexPCT.h"

namespace basecross {

	CircleMarkerObject::CircleMarkerObject(const std::shared_ptr<Stage>& stage)
		:BoardObject(stage, Builder::VertexPCTParametor(Vec3(0.5f, 0.0f, 0.5f), Col4(1.0f, 0.2f , 0.2f, 1.0f), Vec2(512.0f, 512.0f), L"Circle_TX"))
	{}

	void CircleMarkerObject::OnCreate() {
		BoardObject::OnCreate();

		const float SIZE = 6.0f;
		transform->SetScale(Vec3(SIZE));

		auto draw = GetComponent<DrawComp>();
		draw->SetBlendState(BlendState::Opaque);
	}

}

//endbasecross