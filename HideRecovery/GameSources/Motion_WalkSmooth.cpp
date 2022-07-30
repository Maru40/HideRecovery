
/*!
@file Motion_WalkSmooth.cpp
@brief Motion_WalkSmoothのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "Motion_WalkSmooth.h"

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

			WalkSmooth::WalkSmooth(const std::shared_ptr<EnemyBase>& owner)
				:MetaballMotionBase(owner)
			{
				SettingTime(CalculateMotionParametors());
			}

			vector<Metaball::Task::SeekOffsetMove_Parametor> WalkSmooth::CalculateMotionParametors() {
				vector<Metaball::Task::SeekOffsetMove_Parametor> resultParams;

				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (!render) { //NullCheck
					return resultParams;
				}

				//オフセットの取得
				auto offsets = render->CalculateChildrenSeekOffsets();

				Vec3 moveSize = Vec3(-0.15f, -0.25f, 0.25f);
				for (auto& offset : offsets) {
					if (offset.x != 0) {
						offset.x > 0 ? offset.x += moveSize.x : offset.x -= moveSize.x;
					}

					if (offset.y != 0) {
						offset.y > 0 ? offset.y += moveSize.y : offset.y -= moveSize.y;
					}

					if (offset.z != 0) {
						offset.z > 0 ? offset.z += moveSize.z : offset.z -= moveSize.z;
					}
				}

				const float time = 0.4f;
				const float Radius = GetInitializeRadius() * 0.75f;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(20.0f, 0.0f, offsets, SeekTarget::SeekType::Lerp);
				auto parametor = Metaball::Task::SeekOffsetMove_Parametor(time, Radius);
				parametor.seekParams = seekParams;

				//コアのパラメータを設定
				parametor.coreSeekParam = SeekTarget::Parametor(10.0f, 0.0f, Vec3(0.0f, 0.0f, +0.5f), SeekTarget::SeekType::Lerp);

				resultParams.push_back(parametor);

				return resultParams;
			}

		}

	}
}