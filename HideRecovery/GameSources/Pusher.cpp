/*!
@file Pusher.cpp
@brief Pusher�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Pusher.h"

#include "PressData.h"
#include "PressCenterManager.h"
#include "I_Pressed.h"

#include "VelocityManager.h"

#include "GameStageBase.h"
#include "SlowTimerManager.h"

#include "PlayerController.h"
#include "SeekTarget.h"

#include "I_Pusher.h"
#include "I_KnockBack.h"

#include "PlayerController.h"

#include "TimeHelper.h"
#include "MainCameraObject.h"
#include "CameraShake.h"
#include "ChangeCameraManager.h"
#include "TackleCameraObject.h"

#include "AutoReflection.h"

#include "I_Damaged.h"
#include "PlayerStatusManager.h"

#include "InvolvedManager.h"
#include "InvolvedObjects.h"

#include "EffectManager.h"

#include"UtilityObstacle.h"

#include "SingletonComponent.h"
#include "GameManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �ׂ��^�[�Q�b�g�f�[�^
	//--------------------------------------------------------------------------------------

	PushTargetData::PushTargetData()
		:PushTargetData(nullptr, CollisionPair())
	{}

	PushTargetData::PushTargetData(const std::shared_ptr<GameObject>& target, const CollisionPair& collisionPair)
		:target(target), collisionPair(collisionPair)
	{}

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	Pusher::Parametor::Parametor()
		:pushPower(10.0f), data(new PressData)
	{}

	//--------------------------------------------------------------------------------------
	/// �ׂ��Җ{��
	//--------------------------------------------------------------------------------------

	Pusher::Pusher(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void Pusher::OnCreate() {
		AddReactiveIsPusherFunction(false, [&, this]() { StartDeseleration(); });
	}

	void Pusher::OnUpdate() {
		CheckTackleState();   //�^�b�N���X�e�[�g�Ď�
		MissPressUpdate();    //�ׂ����s���A�b�v�f�[�g
		PressingUpdate();     //�ׂ��������A�b�v�f�[�g
		PushTargetUpdate();   //�^�b�N�����A�b�v�f�[�g
	}

	void Pusher::OnDestroy() {
		m_param.destroyAction.Invoke(*this);
		m_param.destroyAction.Clear();
	}

	void Pusher::CheckTackleState() {
		auto stage = GetGameStage();
		if (!stage) {  //�^�[�Q�b�g�����݂��Ȃ��A���̓X�e�[�W�����݂��Ȃ��Ȃ��΂��B
			return;
		}

		auto player = stage->GetPlayer()->GetComponent<PlayerController>(false);
		if (player && !player->IsTackle()) { //�^�b�N���I�����͏I���B
			for (auto data : m_targetDatas) {
				EndPush(data.target.GetShard());
			}

			m_targetDatas.clear();
			SetIsPush(false);
		}
	}

	void Pusher::MissPressUpdate() {
		if (!m_param.isMissPressing) {
			return;
		}

		auto delta = App::GetApp()->GetElapsedTime();
		MoveUpdate(m_param.missPressSpeed, transform->GetForward());
		m_param.missPressSpeed += -m_param.missDeselerationPower * delta;
		if (m_param.missPressSpeed < 0.0f) {
			m_param.missPressSpeed = 0.0f;
			m_param.isMissPressing = false;

			if (auto playerController = GetGameObject()->GetComponent<PlayerController>(false)) {
				playerController->SetUpdateActive(false);
				playerController->SetUpdateActive(true);
			}
		}
	}

	void Pusher::MoveUpdate(const float& speed, const Vec3& direct) {
		auto delta = App::GetApp()->GetElapsedTime();

		//�ړ�����
		auto position = transform->GetPosition();
		position += direct.GetNormalized() * speed * delta;

		transform->SetPosition(position);

		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityManager) {
			velocityManager->ResetAll();
		}
	}

	void Pusher::PressingUpdate() {
		if (!m_param.isPushing) {
			return;
		}

		if (!SlowTimerManager::GetInstance()->IsSlow() && !HitStopManager::GetInstance()->IsHitStop()) {
			m_param.isPushing = false;
			return;
		}
	}

	void Pusher::PushTargetUpdate() {
		for (const auto& data : m_targetDatas) {
			if (!data.target) {
				return;
			}

			auto knockBack = data.target->GetComponent<I_KnockBack>(false);
			if (knockBack) {
				const auto& pair = data.collisionPair;
				auto targetPosition = data.target->GetComponent<Transform>()->GetPosition();

				auto direct = targetPosition - pair.m_CalcHitPoint;
				knockBack->KnockBack(KnockBackData(m_param.pushPower, pair.m_CalcHitPoint, direct));

				//�������݃I�u�W�F�N�g�Ȃ�
				if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
					//�����Veloicty���������̑��x�ɍ��킹��B
					auto velocityManager = data.target->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						auto& selfVelocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
						velocityManager->SetVelocity(selfVelocityManager->GetVelocity());
					}
				}
			}
		}
	}

	void Pusher::StartPush(const CollisionPair& pair) {
		auto target = pair.m_Dest.lock()->GetGameObject();
		if (!IsAddTarget(target) || !IsPush()) { //�ǉ�����^�[�Q�b�g�łȂ��A���́A�v�b�V���ł��Ȃ��Ȃ珈�����΂��B
			return;
		}

		auto press = target->GetComponent<PressCenterManager>(false);
		if (press && press->GetUpdateActive()) {
			press->SetIsPressed(true);
			press->SetPusher(GetThis<Pusher>());
			AddPushTargetData(PushTargetData(target, pair));
		}
	}

	void Pusher::EndPush(const std::shared_ptr<GameObject>& target) {
		if (target == nullptr) {
			return;
		}

		auto press = target->GetComponent<PressCenterManager>(false);
		if (press && press->GetUpdateActive()) {
			press->SetIsPressed(false);
			press->SetPusher(nullptr);
			SetIsPush(false);
		}
	}

	bool Pusher::IsPush() const {
		auto player = GetGameObject()->GetComponent<PlayerController>(false);
		if (player) {  //player�Ȃ�
			return player->IsTackle(); //�^�b�N����ԂȂ�ture
		}

		return m_param.isPush;  //Push��Ԃ��ǂ���
	}

	bool Pusher::IsAddTarget(const std::shared_ptr<GameObject>& addTarget) const {
		if (addTarget->GetComponent<Pusher>(false)) { //Pusher��������Ȃ�
			return false;
		}

		//�����|�C���^�Ȃ�ǉ����Ȃ��B
		for (auto& data : m_targetDatas) {
			const auto& target = data.target;
			if (!target) {
				continue;
			}

			if (target.GetShard() == addTarget) { //�����|�C���^�Ȃ�擾���Ȃ��B
				return false;
			}
		}

		return true;
	}

	void Pusher::HitPusher(const CollisionPair& pair) {
		auto player = GetGameObject()->GetComponent<PlayerController>(false);
		if (!player || !player->IsTackle()) { //Player�ł͂Ȃ��A���́A�^�b�N����ԂłȂ�
			return;
		}

		auto other = pair.m_Dest.lock()->GetGameObject();
		auto pusher = other->GetComponent<Pusher>(false);
		if (pusher) {
			pusher->SetIsPush(true);

			//���x�̉��Z
			auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
			auto otherVelocityManager = other->GetComponent<VelocityManager>(false);
			if (otherVelocityManager && velocityManager) {
				otherVelocityManager->SetVelocity(velocityManager->GetVelocity());
			}
		}
	}

	void Pusher::StartDeseleration() {
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityManager) {
			velocityManager->StartDeseleration();
		}
	}

	void Pusher::HitPushingObstacle(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock();
		//��Q���łȂ�������
		if (!maru::UtilityObstacle::IsObstacle(other->GetGameObject())) {
			return;
		}

		//�I���Ȃ�
		GameManager::GetInstance()->CheckClear();

		//Slow����
		SlowTimerManager::GetInstance()->ForceStop();
		//HitStop�J�n
		const float StopTime = 0.1f;
		HitStopManager::GetInstance()->StartTimer(StopTime);

		//�J�����V�F�C�N
		const float ShakeTime = 0.3f;
		GetGameStage()->GetMainCameraObject()->GetComponent<CameraShake>()->StartShake(ShakeTime, maru::DeltaType::Real);
		GetGameStage()->GetTackleCameraObject()->GetComponent<CameraShake>()->StartShake(ShakeTime, maru::DeltaType::Real);

		//VelocityManager�����Z�b�g
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityManager) {
			velocityManager->ResetAll();
		}

		//�G�t�F�N�g�̐���
		auto effectManager = EffectManager::GetInstance();
		if (effectManager) {
			auto toSelfVec = transform->GetPosition() - pair.m_CalcHitPoint;
			auto quat = maru::Utility::ConvertForwardToQuat(toSelfVec, pair.m_CalcHitPoint);
			effectManager->Play(EffectManager::ID::JetSpake, pair.m_CalcHitPoint, quat);
		}

		//�������݃I�u�W�F�N�g�Ȃ�ϋv�x������B
		if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
			constexpr int damageValue = 1;
			involvedObject->Damaged(damageValue);
			m_param.isPushing = false;
		}

		//MissPressed�Ȃ�
		if (m_param.isMissPressing) {
			m_param.isMissPressing = false;
			auto statusManager = GetGameObject()->GetComponent<PlayerStatusManager>(false);
			if (statusManager) {
				const float& debuffRate = m_param.energyDebuffRate;
				const float& debuffTime = m_param.energyDebuffTime;
				statusManager->EnergyRateTime(debuffRate, debuffTime);
			}
		}
	}

	void Pusher::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock();
		auto pressedManager = other->GetGameObject()->GetComponent<PressCenterManager>(false);
		if (IsPush()&& pressedManager && pressedManager->GetUpdateActive()) {
			StartPush(pair);
		}

		HitPusher(pair);

		//�ׂ����ɏ�Q���Ƀq�b�g
		if (m_param.isPushing) {
			HitPushingObstacle(pair);
		}

		//�ׂ����s
		if (m_param.isMissPressing) {
			HitPushingObstacle(pair);
		}
	}

	void Pusher::OnCollisionExcute(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock();
		auto pressedManager = other->GetGameObject()->GetComponent<PressCenterManager>(false);
		if (IsPush() && pressedManager && pressedManager->GetUpdateActive()) {
			StartPush(pair);
		}

		HitPusher(pair);
	}

	float Pusher::GetTackleSpeed() const {
		auto playerController = GetGameObject()->GetComponent<PlayerController>(false);
		if (!playerController) {
			playerController = GetGameStage()->GetPlayer()->GetComponent<PlayerController>();
		}

		return playerController ? playerController->GetTackleSpeed() : 0.0f;
	}

	void Pusher::PushBack(const Vec3& backVec) {
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (!velocityManager) {  //�R���|�[�l���g�����݂��Ȃ��Ȃ珈�������Ȃ��B
			return;
		}

		velocityManager->ResetAll();
		velocityManager->AddForce(backVec);
	}

	void Pusher::SuccessPressing(const PressData& data) {
		m_param.isPushing = true;

		auto energy = PressedEnergy(0.0f);
		for (auto& targetData : m_targetDatas) {
			const auto& target = targetData.target;
			if (!target) { 
				continue; 
			}

			//�q�b�g�|�C���g�ɑ΂��ď������΂����ꍇ
			auto hitPoint = data.collisionPair.m_CalcHitPoint;
			if (!HitTest::SEGMENT_OBB(transform->GetPosition(), hitPoint, target->GetComponent<CollisionObb>()->GetObb())) {
				EndPush(target.GetShard());
				continue;
			}

			//PressCenterManager�Œׂ��邩���f
			auto pressCenterManager = target->GetComponent<PressCenterManager>(false);
			if (pressCenterManager && pressCenterManager->GetUpdateActive()) {
				auto pressType = pressCenterManager->CalculatePressType(data.collisionPair);
				if (pressType == PressType::Miss) { //�ׂ��Ȃ�������R���e�j���[
					EndPush(target.GetShard());
					continue;
				}
			}
			
			//�ׂ�����
			auto pressed = target->GetComponent<I_Pressed>(false);
			if (pressed) {
				auto newData = PressData(data);
				newData.pressed = target;
				pressCenterManager->SetSuccessPressData(newData);  //�ׂ��f�[�^��ǉ�
				energy += pressed->Pressed(newData); //�ׂ�����

				//�q���ɂ���B
				//target->SetParent(GetGameObject());
			}
		}

		//Pusher�ɃG�l���M�[��n��
		if (auto iPusher = GetGameObject()->GetComponent<I_Pusher>(false)) {
			iPusher->Press(energy);
		}

		//�������݃I�u�W�F�N�g�Ȃ�Player�Ƀf�[�^��n���B
		if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
			GetGameStage()->GetPlayer()->GetComponent<Pusher>(false)->SetSuccessPressData(data);
		}

		//SlowTimerStart
		SlowTimerManager::GetInstance()->StartTimer(data.slowParam.time, data.slowParam.timeSpeed);

		//�J�����̕ύX
		auto changeCamera = ChangeCameraManager::GetInstance();
		changeCamera->GetRefBlackBoardParam().lookAtTarget = GetGameObject();
		changeCamera->ChangeState(ChangeCameraManager::State::Tackle);

		*m_param.data.get() = data;  //�f�[�^�̋L�^
		m_targetDatas.clear(); //�^�[�Q�b�g�̃N���A
	}

	void Pusher::MissPressing(const CollisionPair& pair) {
		if (m_param.isMissPressing) {
			return;
		}

		m_param.isMissPressing = true;
		m_param.missPressSpeed = GetTackleSpeed();
		m_param.missDeselerationPower = GetTackleSpeed() / m_param.missDeselerationTime;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void Pusher::AddReactiveIsPusherFunction(const bool b, const std::function<void()>& function) {
		m_param.isPush.AddFunction(b, function);
	}

	void Pusher::SetSuccessPressData(const PressData& data) {
		*m_param.data.get() = data;
	}

	PressData Pusher::GetSuccessPressData() const {
		return *m_param.data.get();
	}

	void Pusher::AddDestroyAction(const std::function<void(Pusher&)>& action) {
		m_param.destroyAction += action;
	}

}

//endbasecross