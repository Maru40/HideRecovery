/*!
@file PressCenterManager.cpp
@brief PressCenterManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Damaged.h"

#include "I_Pressed.h"
#include "I_Pusher.h"

#include "PlayerStatusManager.h"

#include "GameManager.h"
#include "GameTimer.h"

#include "Animator_Player.h"

#include "DebugObject.h"
#include "TimeManager.h"
#include "PlayerController.h"

#include "EffectManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// プレイヤーステータス
	//--------------------------------------------------------------------------------------

	PlayerStatus::PlayerStatus()
		:PlayerStatus(100.0f, 100.0f, 40.0f, 90.0f, false)
	{}

	PlayerStatus::PlayerStatus(const float& maxHP, const float& energy, const float& energyRecovery, const float& energyUsage, const bool& isUnrivaled):
		hp(maxHP), maxHP(maxHP),
		energy(energy), energyRecovery(energyRecovery), energyUsage(energyUsage),
		isUnrivaled(isUnrivaled)
	{}

	//--------------------------------------------------------------------------------------
	/// プレイヤーステータス管理
	//--------------------------------------------------------------------------------------

	PlayerStatusManager::PlayerStatusManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_status(PlayerStatus()), m_timer(new GameTimer(0.0f)), m_damageIntervalTime(3.0f)
	{}

	void PlayerStatusManager::OnUpdate() {
		m_timer->UpdateTimer();
		if (m_timer->IsTimeUp()) {
			m_status.isUnrivaled = false;
		}
		
		RecoveryEnergy();
	}

	void PlayerStatusManager::Damaged(const DamageData& data) {
		if (m_status.hp <= 0) {
			return;
		}

		if (!m_status.isUnrivaled)
		{
			//ダメージパーティクル
			if(auto effectManager = EffectManager::GetInstance()){
				effectManager->Play(EffectManager::ID::TackleDamage, data.hitPoint);
			}

			// HPを減らす
			m_status.hp -= data.value;
			// HPが0以下ならゲームオーバー
			if (m_status.hp <= 0) {
				m_status.hp = 0;
				//死亡アニメーションに変更
				auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
				if (animator) {
					animator->ForceChangeState(PlayerAnimator::StateEnum::Death);
				}

				//コントローラーOff
				auto controller = GetGameObject()->GetComponent<PlayerController>();
				if (controller) {
					controller->SetUpdateActive(false);
				}

				return;
			}

			// ノックバック
			auto transComp = this->GetGameObject()->GetComponent<Transform>();
			auto pos = transComp->GetPosition();
			pos = pos + data.knockVec * data.knockPower;
			transComp->SetPosition(pos);

			//ダメージインターバルカウント
			m_status.isUnrivaled = true;
			m_timer->ResetTimer(m_damageIntervalTime);

			//ダメージアニメーション再生
			auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
			if (animator) {
				animator->ChangeState(PlayerAnimator::StateEnum::Damage);
			}

			//ダメージ中の行動不能
			auto timer = GetGameObject()->GetComponent<TimeManager>(false);
			if (timer)
			{
				auto controller = GetGameObject()->GetComponent<PlayerController>();
				if (controller)
				{
					controller->SetUpdateActive(false);
					const std::function<void()>& func = [&, controller]() { controller->SetUpdateActive(true); };
					auto data = TimeData(L"Dameged", 1.0f, func);
					timer->AddTimer(data);
				}
			}
		}
	}

	void PlayerStatusManager::ReduceEnergy(float energy)
	{
		m_status.energy -= energy; //エネルギー消費

		auto time = this->GetGameObject()->GetComponent<TimeManager>(false);
		if (time) //ヌルチェック
		{
			auto data =  TimeData(L"EnergyRecoverFlag", 0.7f, &m_RecoverEnergy);
			time->ReSetTimer(data);
		}
		m_RecoverEnergy = false;
	}

	void PlayerStatusManager::EnergyRateTime(float rate, float time)
	{
		SetEnergyRecoverRate(rate); //レートを設定

		auto timer = GetGameObject()->GetComponent<TimeManager>();
		if (timer)
		{
			// 時間経過後レートを元に戻す
			auto data = TimeData(L"EnergyRateTime", time, [&]() {SetEnergyRecoverRate(1.0f); });
			timer->AddTimer(data);
		}
	}

	void PlayerStatusManager::RecoveryEnergy()
	{
		if (m_RecoverEnergy)
		{
			// アプリの参照取得
			auto& app = App::GetApp();

			// 単位時間の取得
			float deltaTime = app->GetElapsedTime();

			if (m_status.energy < 100.0f)
			{
				m_status.energy += m_status.energyRecovery * deltaTime * m_EnergyRecoverRate; // エネルギーゲージ回復
			}

			if (m_status.energy > 100.0f) // 上限値を越えた場合は補正を行う
			{
				m_status.energy = 100.0f;
			}
			else if (m_status.energy < 0.0f) // 下限値を下回った場合も同様に補正を行う
			{
				m_status.energy = 0.0f;
			}
		}
	}

	void PlayerStatusManager::Press(const PressedEnergy& energy) {
		m_status.energy += energy.value;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void PlayerStatusManager::SetStatus(const Status& status) {
		m_status = status;
	}

	PlayerStatusManager::Status PlayerStatusManager::GetStatus() const {
		return m_status;
	}

	bool PlayerStatusManager::IsDeath() const {
		return m_status.hp <= 0;
	}

	bool PlayerStatusManager::IsUseEnergy() const {
		return m_status.energy > 0; //エネルギーが0より大きいなら
	}

}

//endbasecross