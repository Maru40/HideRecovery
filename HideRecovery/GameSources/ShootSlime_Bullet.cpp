/*!
@file ShootSlime_Bullet.cpp
@brief ShootSlime_Bullet�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Damaged.h"
#include "BulletBase.h"
#include "ShootSlime_Bullet.h"

#include "MetaballRenderBase.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "SeekTarget.h"
#include "MetaballChildrenSeekManager.h"

#include "UtilityObstacle.h"

#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "TimerDestroy.h"
#include "VelocityManager.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------

			Bullet_Parametor::Bullet_Parametor()
				:Bullet_Parametor(DamageData(25.0f), 0.3f)
			{}

			Bullet_Parametor::Bullet_Parametor(const DamageData& damageData, const float& destroyTime)
				:damageData(damageData), destroyTime(destroyTime)
			{}

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̒e
			//--------------------------------------------------------------------------------------

			Bullet::Bullet(const std::shared_ptr<GameObject>& objPtr)
				:Bullet(objPtr, Parametor())
			{}

			Bullet::Bullet(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
				: basecross::BulletBase(objPtr), m_param(parametor), m_isActive(false)
			{}

			void Bullet::OnCollisionEnter(const CollisionPair& pair) {
				if (!m_isActive) {
					return;
				}

				auto other = pair.m_Dest.lock();
				if (IsCollision(pair)) {
					//��Q���̎����
					DestroyProcess();
					return;
				}

				auto damaged = other->GetGameObject()->GetComponent<I_Damaged>(false);
				if (damaged) {
					auto damageData = m_param.damageData;
					damageData.hitPoint = pair.m_CalcHitPoint;
					damaged->Damaged(damageData);
					DestroyProcess();
				}
			}

			bool Bullet::IsCollision(const CollisionPair& pair) {
				auto other = pair.m_Dest.lock();

				//�X���C���Ȃ瓖����Ȃ��B
				auto slime = other->GetGameObject()->GetComponent<SlimeBase>(false);
				if (slime) {
					return false;
				}

				//��Q���Ȃ瓖����
				return maru::UtilityObstacle::IsObstacle(other->GetGameObject(), maru::UtilityObstacle::DEFAULT_OBSTACLE_AND_FLOOR_TAGS);
			}

			void Bullet::DestroyProcess() {
				auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
				auto seekManager = GetGameObject()->GetComponent<Metaball::ChildrenSeekManager>(false);

				if (seekManager && velocityManager) {
					seekManager->Crash(velocityManager->GetVelocity());
				}

				GetGameObject()->AddComponent<TimerDestroy>(m_param.destroyTime);
			}

			void Bullet::Shoot(const float& speed, const Vec3& direct) {
				//���x��ǉ�
				auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					auto copyDirect = direct;
					velocityManager->SetVelocity(copyDirect.GetNormalized() * speed);
				}

				//�Ǐ]����
				auto seek = GetGameObject()->GetComponent<SeekTarget>(false);
				if (seek) {
					seek->SetUpdateActive(false);
				}

				m_isActive = true;
			}

		}
	}
}