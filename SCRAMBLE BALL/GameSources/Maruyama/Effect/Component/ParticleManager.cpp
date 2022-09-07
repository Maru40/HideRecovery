/*!
@file ParticleManager.cpp
@brief ParticleManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ParticleManager.h"

#include "Maruyama/UI/3D/Object/BoardObject.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/BillBoard.h"
#include "Maruyama/Utility/Component/BillBoard_Ex.h"

#include "Maruyama/Utility/Random.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/Mathf.h"

#include "Action.h"
#include "Maruyama/Utility/Component/TimerDestroy.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Utility/Component/RangeDestroyManager.h"
#include "Maruyama/Utility/MaruHitHelper.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		ParticleManager_Parametor::ParticleManager_Parametor() :
			ParticleManager_Parametor(Builder::VertexPCTParametor(Vec2(512.0f), L""))
		{}

		ParticleManager_Parametor::ParticleManager_Parametor(const Builder::VertexPCTParametor& vertexPCTParam) :
			ParticleManager_Parametor(
				vertexPCTParam,
				Vec2(20.0f, 40.0f),
				Vec3(2.0f, 2.0f, 0.0f),
				Vec2(1.0f, 2.0f),
				0.75f,
				0.0f,
				20,
				true
			)
		{}

		ParticleManager_Parametor::ParticleManager_Parametor(
			const Builder::VertexPCTParametor& vertexPCTParam,
			const Vec2& powerRandomVec,
			const Vec3& shotDirectRandomVec,
			const Vec2& sizeRandomVec,
			const float& liveTime,
			const float& intervalTime,
			const uint16_t numOneFrameCreate,
			const bool isLoop
		) :
			vertexPCTParam(vertexPCTParam),
			powerRandomVec(powerRandomVec),
			shotDirectRandomVec(shotDirectRandomVec),
			sizeRandomVec(sizeRandomVec),
			liveTime(liveTime),
			intervalTime(intervalTime),
			gravityScale(Vec3(0.0f, -9.8f, 0.0f)),
			numOneFrameCreate(numOneFrameCreate),
			isLoop(isLoop),
			isBillBoard(true),
			isForwardRotation(false),
			isAlphaFade(false),
			isGravity(false),
			isRayHitDestory(false),
			parent(nullptr)
		{}

		//--------------------------------------------------------------------------------------
		/// パーティクル管理本体
		//--------------------------------------------------------------------------------------

		ParticleManager::ParticleManager(const std::shared_ptr<GameObject>& objPtr) :
			ParticleManager(objPtr, Parametor())
		{}

		ParticleManager::ParticleManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
			Component(objPtr), m_param(parametor), m_loopCount(0), m_timer(new GameTimer(0.0f))
		{}

		void ParticleManager::OnCreate() {
			if (!m_param.isLoop) {
				GetGameObject()->AddComponent<TimerDestroy>(m_param.liveTime);
			}

			m_timer->ResetTimer(m_param.intervalTime);
		}

		void ParticleManager::OnUpdate() {
			if (!GetDrawActive() || !GetGameObject()->GetDrawActive()) {  //DrawActiveがfalseなら処理をしない。
				ParticlesUpdate();
				return;
			}

			//一度目なら
			if (m_loopCount == 0) {
				UpdateProcess();
			}
			else { //二度目以降なら
				//ループするなら
				if (m_param.isLoop) {
					LoopUpdate();
				}
			}

			ParticlesUpdate();
		}

		void ParticleManager::OnDrawActive() {
			m_timer->ResetTimer(m_param.intervalTime);
			UpdateProcess();
		}

		void ParticleManager::OnDestroy() {

		}

		void ParticleManager::UpdateProcess() {
			CreateParticles();
			m_loopCount++;
		}

		void ParticleManager::LoopUpdate() {
			m_timer->UpdateTimer();
			
			if (m_timer->IsTimeUp()) {
				UpdateProcess();
				m_timer->ResetTimer(m_param.intervalTime);
			}
		}

		void ParticleManager::ParticlesUpdate() {
			if (m_param.isAlphaFade) {
				AlphaFadeUpdate();
			}
		}

		void ParticleManager::AlphaFadeUpdate() {
			for (auto& particle : m_particels) {
				if (!particle) {
					continue;
				}

				auto draw = particle->GetComponent<BoardObject::DrawComp>();
				auto timerDestroy = particle->GetComponent<TimerDestroy>(false);
				if (timerDestroy) {
					draw->SetAlpha(timerDestroy->GetIntervalTimeRate());
				}
			}
		}

		void ParticleManager::CreateParticle() {
			//ボードオブジェクトの生成
			auto particle = GetStage()->AddGameObject<BoardObject>(m_param.vertexPCTParam);
			particle->AddComponent<VelocityManager>();
			particle->AddComponent<TimerDestroy>(m_param.liveTime);
			//ビルボードをつけるなら
			if (m_param.isBillBoard) {
				particle->AddComponent<BillBoard_Ex>();
			}
			//グラビティをつけるなら
			if (m_param.isGravity) {
				auto gravity = particle->AddComponent<Gravity>();
				gravity->SetGravity(m_param.gravityScale);
			}

			//ペアレント設定
			if (m_param.parent) {
				particle->SetParent(m_param.parent.GetShard());
			}

			//パーティクルのShot
			ShotParticle(particle);

			//HitDestoryするなら
			if (m_param.isRayHitDestory) {
				SettingRayHitDestory(particle);
			}

			//スケール調整
			auto particleTrans = particle->GetComponent<Transform>();
			auto scale = particleTrans->GetScale();
			particleTrans->SetScale(scale * maru::MyRandom::Random(m_param.sizeRandomVec.x, m_param.sizeRandomVec.y));

			m_particels.push_back(particle); //メンバに保存
		}

		void ParticleManager::CreateParticles() {
			const uint16_t& NumOneFrameCreate = m_param.numOneFrameCreate;
			for (int i = 0; i < NumOneFrameCreate; i++) {
				CreateParticle();
			}
		}

		void ParticleManager::SettingRayHitDestory(const std::shared_ptr<BoardObject>& particle) {
			constexpr float SearchRange = 50.0f;
			auto particleTrans = particle->GetComponent<Transform>();
			auto forward = transform->GetForward();
			auto startPosition = transform->GetWorldPosition();
			auto endPosition = startPosition + (forward * SearchRange);

			auto nearObject = maru::UtilityObstacle::FindRayHitNearObstacle(
				startPosition, endPosition, 
				GetStage()->GetGameObjectVec(),
				maru::UtilityObstacle::DEFAULT_OBSTACLE_TAGS
			);

			if (!nearObject) {
				return;
			}

			auto toNearObjectVec = maru::Utility::CalcuToTargetVec(particle, nearObject);
			auto length = toNearObjectVec.length();
			particle->AddComponent<RangeDestoryManager>(toNearObjectVec.length());
		}

		void ParticleManager::ShotParticle(const std::shared_ptr<BoardObject>& particle) {
			auto particleTrans = particle->GetComponent<Transform>();
			particleTrans->SetWorldPosition(transform->GetWorldPosition());
			auto velocityManager = particle->GetComponent<VelocityManager>();
			
			Vec3 direct = CalculateShotDirect();
			float power = maru::MyRandom::Random(m_param.powerRandomVec.x, m_param.powerRandomVec.y);
			velocityManager->SetVelocity(direct.GetNormalized() * power);

			//方向を合わせる
			if (m_param.isForwardRotation) {
				//ビルボードされていなかったら
				auto billBoard = particle->GetComponent<BillBoard_Ex>(false);
				if (!billBoard) {		
					particleTrans->SetLocalForward(direct);
				}
			}
		}

		Vec3 ParticleManager::CalculateShotDirect() {
			auto forward = transform->GetLocalForward();

			const Vec3& randomVec = m_param.shotDirectRandomVec;
			forward = maru::Mathf::Mat4RotationAxisX(forward, XMConvertToRadians(maru::MyRandom::Random(randomVec.x)));
			forward = maru::Mathf::Mat4RotationAxisY(forward, XMConvertToRadians(maru::MyRandom::Random(randomVec.y)));
			forward = maru::Mathf::Mat4RotationAxisZ(forward, XMConvertToRadians(maru::MyRandom::Random(randomVec.z)));

			return forward;
		}

		std::vector<ex_weak_ptr<BoardObject>> ParticleManager::GetParticles() const {
			return m_particels;
		}
	}
}