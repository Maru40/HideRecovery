
/*!
@file Motion_Idle.cpp
@brief Motion_Idleのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "Motion_Idle.h"

#include "EnemyBase.h"
#include "TaskList.h"

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

			//--------------------------------------------------------------------------------------
			/// 追従データ
			//--------------------------------------------------------------------------------------

			SeekData::SeekData()
				:SeekData(30.0f, SeekTarget::SeekType::Lerp)
			{}

			SeekData::SeekData(const float& speed, const SeekTarget_Type& seekType)
				:speed(speed), seekType(seekType)
			{}

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			Idle_Parametor::Idle_Parametor()
				:Idle_Parametor(1.0f)
			{}

			Idle_Parametor::Idle_Parametor(const float& firstSpeed)
				:Idle_Parametor(0.55f, firstSpeed)
			{}

			Idle_Parametor::Idle_Parametor(const float& time, const float& firstSpeed)
				: Idle_Parametor(time, firstSpeed, SeekData(30.0f, SeekTarget::SeekType::Lerp), SeekData(10.0f, SeekTarget::SeekType::Lerp))
			{}

			Idle_Parametor::Idle_Parametor(const float& time, const float& firstSpeed, const SeekData& metaballData, const SeekData& coreData)
				:time(time), firstSpeed(firstSpeed), metaballData(metaballData), coreData(coreData)
			{}

			//--------------------------------------------------------------------------------------
			/// 待機モーション本体
			//--------------------------------------------------------------------------------------

			Idle::Idle(const std::shared_ptr<EnemyBase>& owner)
				:Idle(owner, Parametor())
			{ }

			Idle::Idle(const std::shared_ptr<EnemyBase>& owner, const Parametor& parametor)
				:MetaballMotionBase(owner), m_param(parametor)
			{
				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (render) { //NullCheck
					//オフセットの取得
					m_radius = render->GetChildren()[0]->GetRadius();
					m_offsets = render->CalculateChildrenSeekOffsets();
				}

				SettingTime(CalculateMotionParametors());
			}

			vector<Metaball::Task::SeekOffsetMove_Parametor> Idle::CalculateMotionParametors() {
				vector<Metaball::Task::SeekOffsetMove_Parametor> resultParams;

				const auto& offsets = m_offsets;

				const float Time = m_param.time;
				const auto& metaballData = m_param.metaballData;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(metaballData.speed, 0.0f, offsets, metaballData.seekType);
				auto parametor = Metaball::Task::SeekOffsetMove_Parametor(Time, m_radius);
				parametor.seekParams = seekParams;
				parametor.firstSpeed = m_param.firstSpeed;

				//コアの設定
				const auto& coreData = m_param.coreData;
				parametor.coreSeekParam = SeekTarget::Parametor(coreData.speed, 0.0f, Vec3(0.0f), coreData.seekType);
				 
				resultParams.push_back(parametor);

				return resultParams;
			}

		}
	}
}