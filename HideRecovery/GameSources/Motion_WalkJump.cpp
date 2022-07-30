
/*!
@file Motion_WalkJump.cpp
@brief Motion_WalkJumpのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "Motion_WalkJump.h"

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

			WalkJump::WalkJump(const std::shared_ptr<EnemyBase>& owner)
				:MetaballMotionBase(owner)
			{
				SettingTime(CalculateMotionParametors());
			}

			vector<Metaball::Task::SeekOffsetMove_Parametor> WalkJump::CalculateMotionParametors() {
				vector<Metaball::Task::SeekOffsetMove_Parametor> resultParams;

				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (!render) { //NullCheck
					return resultParams;
				}

				//オフセットの取得
				auto offsets = render->CalculateChildrenSeekOffsets();

				Vec3 moveSize = Vec3(-0.25f, 0.3f, -0.25f);
				//Vec3 moveSize = Vec3(-0.25f, -0.25f, 0.35f);
				const float JumpMagnification = 4.0f;
				float jumpSize = moveSize.y * JumpMagnification;
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

					offset.y += jumpSize;
				}

				const float Time = 0.25f;
				const float Radius = 0.75f;
				const float Speed = 30.0f;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(Speed, 0.0f, offsets, SeekTarget::SeekType::Lerp);
				auto parametor = Metaball::Task::SeekOffsetMove_Parametor(Time, Radius);
				parametor.seekParams = seekParams;
				parametor.firstSpeed = 3.0f;

				//コアの設定
				parametor.coreSeekParam = SeekTarget::Parametor(Speed, 0.0f, Vec3(0.0f, 2.0f, 0.0f), SeekTarget::SeekType::Lerp);

				resultParams.push_back(parametor);

				return resultParams;
			}

		}

	}
}