
/*!
@file SlimeRender.cpp
@brief SlimeRenderのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "BcPNTSlimeDraw.h"

#include "GameStageBase.h"
#include "RayMarchingFilter.h"
#include "MetaballChildrenRender.h"
#include "SeekTarget.h"
#include "VelocityManager.h"
#include "WaitTimer.h"
#include "HitStopManager.h"

#include "SoundManager.h"
#include "TimeHelper.h"

namespace basecross {
	namespace Enemy {

		SlimeRender::SlimeRender(const std::shared_ptr<GameObject>& objPtr)
			:SlimeRender(objPtr, 0)
		{}

		SlimeRender::SlimeRender(const std::shared_ptr<GameObject>& objPtr, const int& numCreate) :
			basecross::Metaball::RenderBase(objPtr, numCreate)
		{}

	}
}

//endbasecross
