/*!
@file MetaballChildrenObject.cpp
@brief MetaballChildrenObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MetaballChildrenObject.h"

#include "MetaballChildrenRender.h"
#include "SeekTarget.h"

#include "VelocityManager.h"

namespace basecross {

	namespace Metaball {

		ChildrenObject::ChildrenObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void ChildrenObject::OnCreate() {
			auto render = AddComponent<ChildrenRender>();
			render->SetRadius(0.25f);
			SetAlphaActive(true);

			auto param = SeekTarget::Parametor();
			param.range = 0;
			param.offset = Vec3(0.0f);
			param.speed = 100.0f;
			param.seekType = SeekTarget::SeekType::Lerp;
			auto seekTarget = AddComponent<SeekTarget>(nullptr, param);

			AddComponent<VelocityManager>();
		}

	}
}

//endbasecross