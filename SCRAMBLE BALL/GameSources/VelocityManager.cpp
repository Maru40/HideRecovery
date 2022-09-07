/*!
@file VelocityManager.h
@brief VelocityManagerステージ
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "VelocityManager.h"
#include "Maruyama/Utility/Mathf.h"
#include "Maruyama/Utility/UtilityVelocity.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Item/HideItem.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/StageObject/Goal.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	VelocityManager::Parametor::Parametor() 
		:Parametor(Vec3(0.0f), Vec3(0.0f), 1.0f, false)
	{}

	VelocityManager::Parametor::Parametor(
		const Vec3& force, 
		const Vec3& velocity, 
		const float& deselerationPower, 
		const bool isDeseleration
	):
		force(force), 
		velocity(velocity), 
		beforeVelocity(velocity),
		deselerationPower(deselerationPower), 
		maxSpeed(FLT_MAX),
		isDeseleration(isDeseleration)
	{}

	//--------------------------------------------------------------------------------------
	///	速度管理
	//--------------------------------------------------------------------------------------

	VelocityManager::VelocityManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void VelocityManager::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		m_param.beforeVelocity = m_param.velocity; //前フレームの速度を保存

		GravityUpdate();

		m_param.velocity += m_param.force * delta; //力を加える

		//上限確認
		auto speed = maru::Mathf::Clamp(m_param.velocity.length(), 0.0f, m_param.maxSpeed);
		m_param.velocity = m_param.velocity.GetNormalized() * speed;

		Move();  //移動処理

		ResetForce();

		Deseleration();	//減速処理

		////デバッグ
		//if (PlayerInputer::GetInstance()->IsRightDown()) {
		//	transform->SetPosition(maru::Utility::FindComponent<HideItem>()->GetGameObject()->GetComponent<Transform>()->GetPosition() + Vec3::Up());
		//}

		////デバッグ
		//if (PlayerInputer::GetInstance()->IsLeftDown()) {
		//	auto goals = maru::Utility::FindComponents<Goal>();
		//	Vec3 position;
		//	for (auto goal : goals) {
		//		if (goal->GetTeam() != GetGameObject()->GetComponent<I_TeamMember>()->GetTeam()) {
		//			position = goal->GetGameObject()->GetComponent<Transform>()->GetPosition();
		//		}
		//	}
		//	transform->SetPosition(position + Vec3::Right());
		//}
	}

	void VelocityManager::Move() {
		if (m_param.velocity == Vec3(0.0f)) {
			return;
		}

		auto delta = App::GetApp()->GetElapsedTime();

		auto pos = transform->GetPosition();
		pos += m_param.velocity * delta;
		transform->SetPosition(pos);
	}

	void VelocityManager::Deseleration() {
		if (!m_param.isDeseleration) {
			return;
		}

		auto velocity = m_param.velocity;
		auto woldVecolisty = GetWorldVelocity();
		auto force = UtilityVelocity::CalucSeekVec(velocity, -velocity, velocity.length() * m_param.deselerationPower);
		AddForce(force);

		constexpr float StopSpeed = 0.3f;
		if (velocity.length() <= StopSpeed) {
			SetIsDeseleration(false);
			ResetVelocity();
		}
	}

	void VelocityManager::GravityUpdate() {
		//重力分加算する。
		auto delta = App::GetApp()->GetElapsedTime();
		auto gravity = GetGameObject()->GetComponent<Gravity>(false);
		if (!gravity) {
			m_param.velocity.y += m_param.force.y * delta;
			return;
		}

		if (m_param.force.y != 0.0f) {
			Debug::GetInstance()->Log(m_param.force);
		}

		auto gravityVelocity = gravity->GetGravityVelocity();
		gravityVelocity.y += m_param.force.y * delta;
		gravity->SetGravityVerocity(gravityVelocity);
		m_param.velocity.y = gravityVelocity.y;

		//y軸のForceのリセット
		m_param.force.y = 0;
	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void VelocityManager::SetVelocity(const Vec3& velocity) noexcept {
		m_param.velocity = velocity;
	}

	Vec3 VelocityManager::GetVelocity() const noexcept {
		return m_param.velocity;
	}

	Vec3 VelocityManager::GetWorldVelocity() const {
		auto velocity = m_param.velocity;
		auto parent = GetGameObject()->GetParent();
		if (parent) {
			if (auto velocityManager = parent->GetComponent<VelocityManager>(false)) {
				velocity += velocityManager->GetWorldVelocity();
			}
		}

		return velocity;
	}

	Vec3 VelocityManager::GetBeforeVelocity() const noexcept {
		return m_param.beforeVelocity;
	}

    void VelocityManager::AddForce(const Vec3& force) noexcept {
        m_param.force += force;
    }

    Vec3 VelocityManager::GetForce() const noexcept {
        return m_param.force;
    }

    void VelocityManager::ResetVelocity() noexcept {
        m_param.velocity = Vec3(0.0f);
        m_param.beforeVelocity = Vec3(0.0f);
    }

    void VelocityManager::ResetForce() noexcept {
        m_param.force = Vec3(0.0f);
    }

    void VelocityManager::ResetAll() noexcept {
        ResetVelocity();
        ResetForce();
    }

    void VelocityManager::StartDeseleration(const float& power) noexcept {
        SetIsDeseleration(true);
        m_param.deselerationPower = power;
    }

    void VelocityManager::SetIsDeseleration(const bool isDeseleration) noexcept {
        m_param.isDeseleration = isDeseleration;
    }

	bool VelocityManager::IsDeseleration() const noexcept {
		return m_param.isDeseleration;
	}

	float VelocityManager::GetDeselerationPower() const noexcept {
		return m_param.deselerationPower;
	}

	void VelocityManager::SetDeseletationPower(const float& power) noexcept {
		m_param.deselerationPower = power;
	}

	void VelocityManager::SetMaxSpeed(const float& speed) noexcept {
		m_param.maxSpeed = speed;
	}

	float VelocityManager::GetMaxSpeed() const noexcept {
		return m_param.maxSpeed;
	}
}

//endbasecross