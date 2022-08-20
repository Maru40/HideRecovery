
/*!
@file UseWepon.cpp
@brief UseWeponクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "UseWepon.h"

#include "WeponBase.h"

#include "ReactiveBool.h"

#include "PlayerInputer.h"

#include "Watanabe/Component/PlayerAnimator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラスのパラメータ
	//--------------------------------------------------------------------------------------
	UseWepon_Parametor::UseWepon_Parametor() :
		UseWepon_Parametor(false)
	{}

	UseWepon_Parametor::UseWepon_Parametor(const bool isAim) :
		isAim(new maru::ReactiveBool(isAim))
	{}

	UseWepon_Parametor::UseWepon_Parametor(const UseWepon_Parametor& parametor) :
		UseWepon_Parametor(parametor.isAim->GetValue())
	{}

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラス本体
	//--------------------------------------------------------------------------------------

	UseWepon::UseWepon(const std::shared_ptr<GameObject>& objPtr) :
		UseWepon(objPtr, nullptr)
	{}

	UseWepon::UseWepon(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<WeponBase>& wepon) :
		Component(objPtr),
		m_param(Parametor()),
		m_wepon(wepon)
	{}

	void UseWepon::OnLateStart() {
		SettingReactiveIsAim();
	}

	void UseWepon::OnUpdate() {
		if (IsAim()) {
			AimUpdate();
		}

		if (PlayerInputer::GetInstance()->IsLBDown()) {
			SetIsAim(true);
		}
		
		if (PlayerInputer::GetInstance()->IsLBUp()) {
			SetIsAim(false);
		}

	}

	void UseWepon::AimUpdate() {

	}

	void UseWepon::SettingReactiveIsAim() {
		auto& isAim = m_param.isAim;

		auto trueFunction = [&]() {	//Aim状態になった時
			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunSet2);
			}
		};

		auto falseFunction = [&]() {	//Aim状態でなくなった時
			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunEnd2);
			}
		};

		isAim->AddFunction(true, trueFunction);
		isAim->AddFunction(false, falseFunction);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void UseWepon::SetIsAim(const bool isAim) { *m_param.isAim = isAim; }

	bool UseWepon::IsAim() const { return m_param.isAim->GetValue(); }

	void UseWepon::SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept { m_wepon = wepon; }

	std::shared_ptr<WeponBase> UseWepon::GetWepon() const noexcept { return m_wepon.lock(); }

}