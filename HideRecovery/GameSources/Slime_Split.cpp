
/*!
@file Slime_Split.cpp
@brief Slime_Splitのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "PressData.h"
#include "Slime_Split.h"

#include "Pusher.h"

#include "SlowTimerManager.h"
#include "HitStopManager.h"
#include "MetaballChildrenPressedManager.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "NormalSlimeObject.h"

#include "MaruUtility.h"
#include "Mathf.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Split::Slime_Split(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{}

			void Slime_Split::OnStart() {
				//分裂用のスライムを生成
				for (auto& position : CalculateCreatePositions()) {
					auto forward = m_paramPtr->pressData.pusher->GetComponent<Transform>()->GetForward();
					CreateSlime(position, forward);
				}

				//潰すのにかかる時間のセット
				auto pressManager = GetOwner()->GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				if (pressManager) {
					m_timer->ResetTimer(pressManager->GetPressedTime());
				}
			}

			bool Slime_Split::OnUpdate() {
				m_timer->UpdateTimer(1.0f, maru::DeltaType::Real);

				return m_timer->IsTimeUp();
			}

			void Slime_Split::OnExit() {

			}

			std::vector<Vec3> Slime_Split::CalculateCreatePositions() {
				std::vector<Vec3> resultVecs;
				auto& pressData = m_paramPtr->pressData;
				auto pusherPosition = pressData.GetPusher()->GetGameObject()->GetComponent<Transform>()->GetPosition();

				//潰している方向の取得
				auto hitPointToPressedVec = pressData.GetHitPointToPressedVec();
				hitPointToPressedVec.y = 0.0f;
				
				//ベクトルを回転
				constexpr float Radian = XMConvertToRadians(30.0f);
				resultVecs.push_back(pusherPosition + maru::Mathf::Mat4RotationAxisY(hitPointToPressedVec, +Radian));
				resultVecs.push_back(pusherPosition + maru::Mathf::Mat4RotationAxisY(hitPointToPressedVec, -Radian));

				return resultVecs;
			}

			std::shared_ptr<GameObject> Slime_Split::CreateSlime(const Vec3& position, const Vec3& forward) {
				auto stage = GetOwner()->GetStage();
				auto slime = stage->AddGameObject<Enemy::NormalSlimeObject>();
				auto slimeTrans = slime->GetComponent<Transform>();
				slimeTrans->SetPosition(position);
				slimeTrans->SetForward(forward);
				
				return slime;
			}

		}
	}
}