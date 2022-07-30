/*!
@file Pusher.cpp
@brief Pusherのクラス実体
担当：丸山裕喜
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
	/// 潰すターゲットデータ
	//--------------------------------------------------------------------------------------

	PushTargetData::PushTargetData()
		:PushTargetData(nullptr, CollisionPair())
	{}

	PushTargetData::PushTargetData(const std::shared_ptr<GameObject>& target, const CollisionPair& collisionPair)
		:target(target), collisionPair(collisionPair)
	{}

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	Pusher::Parametor::Parametor()
		:pushPower(10.0f), data(new PressData)
	{}

	//--------------------------------------------------------------------------------------
	/// 潰す者本体
	//--------------------------------------------------------------------------------------

	Pusher::Pusher(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void Pusher::OnCreate() {
		AddReactiveIsPusherFunction(false, [&, this]() { StartDeseleration(); });
	}

	void Pusher::OnUpdate() {
		CheckTackleState();   //タックルステート監視
		MissPressUpdate();    //潰し失敗時アップデート
		PressingUpdate();     //潰し成功時アップデート
		PushTargetUpdate();   //タックル中アップデート
	}

	void Pusher::OnDestroy() {
		m_param.destroyAction.Invoke(*this);
		m_param.destroyAction.Clear();
	}

	void Pusher::CheckTackleState() {
		auto stage = GetGameStage();
		if (!stage) {  //ターゲットが存在しない、又はステージが存在しないなら飛ばす。
			return;
		}

		auto player = stage->GetPlayer()->GetComponent<PlayerController>(false);
		if (player && !player->IsTackle()) { //タックル終了時は終わる。
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

		//移動処理
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

				//巻き込みオブジェクトなら
				if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
					//相手のVeloictyをこっちの速度に合わせる。
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
		if (!IsAddTarget(target) || !IsPush()) { //追加するターゲットでない、又は、プッシュできないなら処理を飛ばす。
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
		if (player) {  //playerなら
			return player->IsTackle(); //タックル状態ならture
		}

		return m_param.isPush;  //Push状態かどうか
	}

	bool Pusher::IsAddTarget(const std::shared_ptr<GameObject>& addTarget) const {
		if (addTarget->GetComponent<Pusher>(false)) { //Pusherだったら省く
			return false;
		}

		//同じポインタなら追加しない。
		for (auto& data : m_targetDatas) {
			const auto& target = data.target;
			if (!target) {
				continue;
			}

			if (target.GetShard() == addTarget) { //同じポインタなら取得しない。
				return false;
			}
		}

		return true;
	}

	void Pusher::HitPusher(const CollisionPair& pair) {
		auto player = GetGameObject()->GetComponent<PlayerController>(false);
		if (!player || !player->IsTackle()) { //Playerではない、又は、タックル状態でない
			return;
		}

		auto other = pair.m_Dest.lock()->GetGameObject();
		auto pusher = other->GetComponent<Pusher>(false);
		if (pusher) {
			pusher->SetIsPush(true);

			//速度の加算
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
		//障害物でなかったら
		if (!maru::UtilityObstacle::IsObstacle(other->GetGameObject())) {
			return;
		}

		//終了なら
		GameManager::GetInstance()->CheckClear();

		//Slow解除
		SlowTimerManager::GetInstance()->ForceStop();
		//HitStop開始
		const float StopTime = 0.1f;
		HitStopManager::GetInstance()->StartTimer(StopTime);

		//カメラシェイク
		const float ShakeTime = 0.3f;
		GetGameStage()->GetMainCameraObject()->GetComponent<CameraShake>()->StartShake(ShakeTime, maru::DeltaType::Real);
		GetGameStage()->GetTackleCameraObject()->GetComponent<CameraShake>()->StartShake(ShakeTime, maru::DeltaType::Real);

		//VelocityManagerをリセット
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityManager) {
			velocityManager->ResetAll();
		}

		//エフェクトの生成
		auto effectManager = EffectManager::GetInstance();
		if (effectManager) {
			auto toSelfVec = transform->GetPosition() - pair.m_CalcHitPoint;
			auto quat = maru::Utility::ConvertForwardToQuat(toSelfVec, pair.m_CalcHitPoint);
			effectManager->Play(EffectManager::ID::JetSpake, pair.m_CalcHitPoint, quat);
		}

		//巻き込みオブジェクトなら耐久度が減る。
		if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
			constexpr int damageValue = 1;
			involvedObject->Damaged(damageValue);
			m_param.isPushing = false;
		}

		//MissPressedなら
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

		//潰し中に障害物にヒット
		if (m_param.isPushing) {
			HitPushingObstacle(pair);
		}

		//潰し失敗
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
		if (!velocityManager) {  //コンポーネントが存在しないなら処理をしない。
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

			//ヒットポイントに対して処理を飛ばした場合
			auto hitPoint = data.collisionPair.m_CalcHitPoint;
			if (!HitTest::SEGMENT_OBB(transform->GetPosition(), hitPoint, target->GetComponent<CollisionObb>()->GetObb())) {
				EndPush(target.GetShard());
				continue;
			}

			//PressCenterManagerで潰せるか判断
			auto pressCenterManager = target->GetComponent<PressCenterManager>(false);
			if (pressCenterManager && pressCenterManager->GetUpdateActive()) {
				auto pressType = pressCenterManager->CalculatePressType(data.collisionPair);
				if (pressType == PressType::Miss) { //潰せなかったらコンテニュー
					EndPush(target.GetShard());
					continue;
				}
			}
			
			//潰す処理
			auto pressed = target->GetComponent<I_Pressed>(false);
			if (pressed) {
				auto newData = PressData(data);
				newData.pressed = target;
				pressCenterManager->SetSuccessPressData(newData);  //潰すデータを追加
				energy += pressed->Pressed(newData); //潰す処理

				//子供にする。
				//target->SetParent(GetGameObject());
			}
		}

		//Pusherにエネルギーを渡す
		if (auto iPusher = GetGameObject()->GetComponent<I_Pusher>(false)) {
			iPusher->Press(energy);
		}

		//巻き込みオブジェクトならPlayerにデータを渡す。
		if (auto involvedObject = dynamic_pointer_cast<InvolvedObjects>(GetGameObject())) {
			GetGameStage()->GetPlayer()->GetComponent<Pusher>(false)->SetSuccessPressData(data);
		}

		//SlowTimerStart
		SlowTimerManager::GetInstance()->StartTimer(data.slowParam.time, data.slowParam.timeSpeed);

		//カメラの変更
		auto changeCamera = ChangeCameraManager::GetInstance();
		changeCamera->GetRefBlackBoardParam().lookAtTarget = GetGameObject();
		changeCamera->ChangeState(ChangeCameraManager::State::Tackle);

		*m_param.data.get() = data;  //データの記録
		m_targetDatas.clear(); //ターゲットのクリア
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
	/// アクセッサ
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