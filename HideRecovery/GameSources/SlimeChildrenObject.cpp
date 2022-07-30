/*!
@file SlimeChildrenObject.cpp
@brief SlimeChildrenObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MetaballChildrenObject.h"
#include "SlimeChildrenObject.h"

#include "MetaballChildrenRender.h"
#include "SeekTarget.h"

namespace basecross {
	namespace Enemy {

		SlimeChildrenObject::SlimeChildrenObject(const std::shared_ptr<Stage>& stage)
			:basecross::Metaball::ChildrenObject(stage)
		{}

		void SlimeChildrenObject::OnCreate() {
			AddTag(L"Slime");
		}
	}
}