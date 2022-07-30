/*!
@file SegmentMarkerObject.cpp
@brief SegmentMarkerObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SegmentMarkerObject.h"

#include "BuilderVertexPCT.h"

namespace basecross {

	SegmentMarkerObject::SegmentMarkerObject(const std::shared_ptr<Stage>& stage)
		:BoardObject(stage, 
			Builder::VertexPCTParametor(Vec3(1.5f, 0.0f, 1.5f), Col4(1.0f,0.0f,0.0f,1.0f), 
				Vec2(128.0f, 512.0f),
				L"Segment_TX", Builder::UIPivot::Center))
	{}

	void SegmentMarkerObject::OnCreate() {
		BoardObject::OnCreate();

		const auto SCALE = Vec3(0.1f, 1.0f , 1.0f);
		transform->SetScale(SCALE);

		auto draw = GetComponent<DrawComp>();
		draw->SetBlendState(BlendState::Opaque);
	}

}

//endbasecross