
/*!
@file Motion_GenericPreliminary.cpp
@brief Motion_GenericPreliminaryのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "Motion_GenericPreliminary.h"

#include "EnemyBase.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"

#include "MetaballRenderBase.h"

namespace basecross {

	namespace Enemy {

		namespace Motion {

			GenericPreliminary::GenericPreliminary(const std::shared_ptr<EnemyBase>& owner)
				:MetaballMotionBase(owner)
			{
				SettingTime(CalculateMotionParametors());
			}

			vector<Metaball::Task::SeekOffsetMove_Parametor> GenericPreliminary::CalculateMotionParametors() {
				vector<Metaball::Task::SeekOffsetMove_Parametor> resultParams;

				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (!render) { //NullCheck
					return resultParams;
				}

				//オフセットの取得
				auto offsets = GetInitializeOffsets();

				Vec3 moveSize = Vec3(0.25f, 0.0f, 0.35f);
				for (auto& offset : offsets) {
					if (offset.x != 0) {
						offset.x > 0 ? offset.x += moveSize.x : offset.x -= moveSize.x;
					}
					
					if (offset.z != 0) {
						offset.z > 0 ? offset.z += moveSize.z : offset.z -= moveSize.z;
					}
				}

				const auto TransitionTime = 0.45f;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(20.0f, 0.0f, offsets, SeekTarget::SeekType::Lerp);
				const float Radius = GetInitializeRadius() * 0.75f;
				auto parametor = Metaball::Task::SeekOffsetMove_Parametor(TransitionTime, Radius);
				parametor.seekParams = seekParams;

				resultParams.push_back(parametor);

				return resultParams;
			}

		}
	}
}
