
/*!
@file Motion_KnockBack.cpp
@brief Motion_KnockBackのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_KnockBack.h"
#include "MaruAnimationHelper.h"
#include "Motion_KnockBack.h"

#include "EnemyBase.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"

#include "MaruUtility.h"

#include "MetaballRenderBase.h"
#include "MetaballChildrenRender.h"

#include "AbsorbedSlimeManager.h"

#include "StatorBase.h"
#include "SlimeStatorBase.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "Mathf.h"

#include "PressCenterManager.h"

namespace basecross {

	namespace Enemy {

		namespace Motion {

			//パラメータ------------------------------------------------------------------------------------------------------------

			KnockBack_Parametor::KnockBack_Parametor()
				:KnockBack_Parametor(KnockBackData(), 0.75f)
			{}

			KnockBack_Parametor::KnockBack_Parametor(const float& range)
				:KnockBack_Parametor(KnockBackData(), range)
			{}

			KnockBack_Parametor::KnockBack_Parametor(const KnockBackData& data, const float& range)
				: KnockBack_Parametor(data, range, 0.1f)
			{}

			KnockBack_Parametor::KnockBack_Parametor(const KnockBackData& data, const float& range, const float& transitionTime)
				: data(data), range(range), transitionTime(transitionTime), avoidRange(2.0f), maxRangeVec(0.4f)
			{}

			//----------------------------------------------------------------------------------------------------------------------


			KnockBack::KnockBack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<KnockBack::Parametor>& paramPtr)
				:MetaballMotionBase(owner), m_paramPtr(paramPtr) 
			{
				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (render) { //NullCheck
					//オフセットの取得
					m_defaultOffsets = render->CalculateChildrenSeekOffsets();
				}

				SettingTime(CalculateMotionParametors());
			}

			vector<Metaball::Task::SeekOffsetMove_Parametor> KnockBack::CalculateMotionParametors() {
				vector<Metaball::Task::SeekOffsetMove_Parametor> resultParams;

				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (!render) { //NullCheck
					DebugObject::AddString(L"KnockBack::CalculateMotionParametors(), RenderBaseが存在しません。");
					return resultParams;
				}

				//オフセットの取得
				auto offsets = render->CalculateChildrenSeekOffsets();
				auto centerPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				auto quat = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetQuaternion();
				auto forward = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetForward();
				const auto& data = m_paramPtr->data;

				int index = 0;
				for (auto& offset : offsets) {
					auto direct = ConvertRotationCenterToOffset(centerPosition, forward, offset);
					direct += CalculateAvoidVec(direct, index, offsets);

					offset += direct.GetNormalized() * data.power;

					//距離制限
					auto maxRangeVec = maru::Mathf::AbsVec(GetMaxRangeVec());
					auto minRangeVec = -maxRangeVec;
					maxRangeVec += m_defaultOffsets[index];
					minRangeVec += m_defaultOffsets[index];
					offset = maru::Mathf::ClampVec(offset, minRangeVec, maxRangeVec);

					index++;
				}

				const float Time = m_paramPtr->transitionTime;
				const float Radius = GetInitializeRadius() * 0.5f;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(50.0f, 0.0f, offsets, SeekTarget::SeekType::Lerp);
				auto parametor = Metaball::Task::SeekOffsetMove_Parametor(Time, Radius);
				parametor.seekParams = seekParams;

				resultParams.push_back(parametor);

				return resultParams;
			}

			Vec3 KnockBack::ConvertRotationCenterToOffset(const Vec3& centerPosition, const Vec3& forward, const Vec3& offset) {
				auto rotOffset = maru::Utility::ConvertForwardOffset(forward, offset);
				auto position = centerPosition + rotOffset;
				auto direct = position - m_paramPtr->data.hitPoint;
				direct += m_paramPtr->data.direct;
				direct = maru::Utility::ConvertForwardOffset(forward, direct);  //コメントアウト中、後でtest的に使うから消さないように。

				return direct;
			}

			Vec3 KnockBack::CalculateAvoidVec(const Vec3& baseDirect, const int currentIndex, const std::vector<Vec3>& offsets) {
				auto centerPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				auto forward = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetForward();
				const auto& data = m_paramPtr->data;
				auto selfPosition = centerPosition + offsets[currentIndex];

				Vec3 sumVec(0.0f);
				int avoidCount = 0;  //回避した数
				for (int i = 0; i < offsets.size(); i++) {
					if (currentIndex == i) {
						continue;
					}

					const auto& offset = offsets[i];
					Vec3 otherPosition(0.0f);

					//otherPosition = centerPosition + maru::Utility::ConvertForwardOffset(forward, offset); //足されていればそのまま
					otherPosition = ConvertRotationCenterToOffset(centerPosition, forward, offset);

					auto toOtherVec = otherPosition - selfPosition;

					const float avoidRange = m_paramPtr->avoidRange;
					float length = toOtherVec.length();
					if (toOtherVec.length() < avoidRange) {  //回避距離なら。
						sumVec += toOtherVec;
						avoidCount++;
					}
				}

				//回避カウントを一度でもカウントしたら。
				return avoidCount != 0 ? sumVec / (float)avoidCount : Vec3(0.0f);
			}

			Vec3 KnockBack::CalculatePressedVec() {
				if (auto pressedCenterManager = GetOwner()->GetGameObject()->GetComponent<PressCenterManager>(false)) {
					return pressedCenterManager->GetSuccessPressData().pressedVec;
				}

				return Vec3(0.0f);
			}

			float KnockBack::GetRange() const {
				auto ownerObject = GetOwner()->GetGameObject();
				auto range = m_paramPtr->range;

				//Parentがあるなら
				auto render = ownerObject->GetComponent<Metaball::RenderBase>(false);
				if (render && render->IsChildrenParent()) {
					auto parent = render->GetChildrenParent();
					auto scale = parent->GetComponent<Transform>()->GetWorldScale();
					auto average = (scale.x + scale.y + scale.z) / 3.0f;
					range *= average;
				}

				//Absrobed状態なら
				if (IsAbsrobed()) {
					range *= GetNumAbsrobed();
				}

				return range;
			}

			int KnockBack::GetNumAbsrobed() const {
				auto parent = GetOwner()->GetGameObject()->GetParent();
				if (!parent) {
					return 0;
				}

				auto absorbed = parent->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				return absorbed ? absorbed->GetNumAbsrobedObjects() : 0;
			}

			Vec3 KnockBack::GetMaxRangeVec() const {
				auto ownerObject = GetOwner()->GetGameObject();
				auto forward = ownerObject->GetComponent<Transform>()->GetForward();
				auto maxVec = m_paramPtr->maxRangeVec;
				//auto maxVec = maru::Utility::ConvertForwardOffset(forward, m_paramPtr->maxRangeVec);

				//Parentがあるなら
				auto render = ownerObject->GetComponent<Metaball::RenderBase>(false);
				if (render && render->IsChildrenParent()) {
					auto parent = render->GetChildrenParent();
					auto scale = parent->GetComponent<Transform>()->GetWorldScale();
					maxVec = (Vec3)XMVectorMultiply(maxVec, scale);
				}

				//Absrobed状態なら
				if (IsAbsrobed()) {
					for (const auto& offset : GetAbsorbedOffsets()) {
						maxVec += (offset);
					}
				}

				auto length = maxVec.length();
				return maxVec;
			}

			bool KnockBack::IsAbsrobed() const {
				if (auto stator = GetOwner()->GetGameObject()->GetComponent<I_Stator_EnumType<Enemy::SlimeStateType>>(false)) {
					return stator->IsCurrentState(Enemy::SlimeStateType::Absorbed);
				}

				return false;
			}

			std::vector<Vec3> KnockBack::GetAbsorbedOffsets() const {
				std::vector<Vec3> results;
				auto parent = GetOwner()->GetGameObject()->GetParent();
				if (!parent) {
					return results;
				}

				//AbsorbedMangaerをアタッチされているなら
				if (auto absorbed = parent->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false)) {
					for (const auto& offset : absorbed->GetAbsorbedOffsets()) {
						results.push_back(offset); //Offsetを保存
					}
				}

				return results;
			}
		}
	}
}
