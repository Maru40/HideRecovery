/*!
@file ShootManager.cpp
@brief ShootManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ShootManager.h"

#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "SeekTarget.h"
#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"

#include "MetaballChildrenRender.h"
#include "VelocityManager.h"

#include "I_Damaged.h"
#include "BulletBase.h"
#include "ShootSlime_Bullet.h"
#include "ShootSlime_BulletObject.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			ShootChargeData::ShootChargeData()
				:ShootChargeData(0.0f, 0.0f)
			{}

			ShootChargeData::ShootChargeData(const float& time, const float& range)
				:time(time), range(range)
			{}

			//--------------------------------------------------------------------------------------
			/// ショット管理クラス本体
			//--------------------------------------------------------------------------------------

			ShootManager::ShootManager(const std::shared_ptr<GameObject>& objPtr)
				:ShootManager(objPtr, Parametor())
			{}

			ShootManager::ShootManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
				:Component(objPtr), m_param(parametor), m_bulletObject(nullptr)
			{}

			void ShootManager::OnCreate() {
				CrealteBulletObject();
			}

			void ShootManager::OnDestroy() {
				GetStage()->RemoveGameObject<GameObject>(m_bulletObject.GetShard());
			}

			void ShootManager::CrealteBulletObject() {
				auto stage = GetStage();
				auto object = stage->AddGameObject<ShootSlime::BulletObject>();
				auto seek = object->GetComponent<SeekTarget>(false);
				if (seek) {
					seek->SetTarget(GetGameObject());
				}

				m_bulletObject = object;
			}

			void ShootManager::StartCharge(const float& time, const float& range) {
				StartCharge(ShootChargeData(time, range));
			}

			void ShootManager::StartCharge(const ShootChargeData& data) {
				if (m_bulletObject.GetShard() == nullptr) {
					return;
				}

				using TaskEnum = SeekTargetOffsetsManager::TaskEnum;

				auto seekManager = m_bulletObject->GetComponent<Metaball::ChildrenSeekManager>(false);
				if (seekManager) {
					auto parametor = SeekTargetOffsetsManager::Parametor();
					parametor.time = data.time;
					parametor.range = data.range;
					seekManager->StartMove(TaskEnum::CenterMove, parametor);
				}
			}

			void ShootManager::Break() {
				if (m_bulletObject.GetShard() == nullptr) {
					return;
				}

				auto bullet = m_bulletObject->GetComponent<ShootSlime::Bullet>();
				bullet->DestroyProcess();

				CrealteBulletObject();
			}

			bool ShootManager::IsEndCharge() const {
				if (m_bulletObject.GetShard() == nullptr) {
					return true;
				}

				auto seekManager = m_bulletObject->GetComponent<Metaball::ChildrenSeekManager>(false);
				if (seekManager) {
					return seekManager->IsEndTask();
				}

				return true;
			}

			void ShootManager::Shoot(const float& speed, const std::shared_ptr<GameObject>& target) {
				auto targetPosition = target->GetComponent<Transform>()->GetPosition();
				Shoot(speed, targetPosition);
			}

			void ShootManager::Shoot(const float& speed, const Vec3& targetPosition) {
				if (m_bulletObject.GetShard() == nullptr) {
					return;
				}

				auto bullet = m_bulletObject->GetComponent<ShootSlime::Bullet>(false);
				if (bullet) {
					auto shootPosition = m_bulletObject->GetComponent<Transform>()->GetPosition();
					auto direct = targetPosition - shootPosition;

					bullet->Shoot(speed, direct);
				}

				m_bulletObject = nullptr;
			}

			void ShootManager::Reload() {
				CrealteBulletObject();
			}
		}
	}
}