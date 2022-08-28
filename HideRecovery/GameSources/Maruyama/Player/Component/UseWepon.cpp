
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

#include "Itabashi/ObjectMover.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "RotationController.h"
#include "VelocityManager.h"
#include "MaruUtility.h"
#include "EyeSearchRange.h"

#include "Itabashi/GamePlayerManager.h"

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
		m_wepon(wepon),
		m_readyArmsSoundClip(L"ReadyArmsSE", false, 0.75f)
	{}

	void UseWepon::OnLateStart() {
		m_rotationController = GetGameObject()->GetComponent<RotationController>(false);
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		m_animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>(false);

		SettingReactiveIsAim();
	}

	void UseWepon::OnUpdate() {
		if (IsAim()) {
			AimUpdate();
		}
	}

	void UseWepon::AimUpdate() {
		RotationUpdate();
		AnimationUpdate();
	}

	void UseWepon::AnimationUpdate() {
		auto velocityManager = m_velocityManager.lock();
		auto animator = m_animator.lock();
		if (!velocityManager || !animator) {
			return;
		}

		if (animator->IsCurretAnimationState(PlayerAnimationState::State::GunSet2)) {	//GunSet2の間なら処理を飛ばす。
			return;
		}

		auto velocity = velocityManager->GetVelocity();
		velocity.y = 0.0f;

		constexpr float StopVelocity = 0.01f;
		if (velocity.length() <= StopVelocity) {	//止まっていると判断する速度なら
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Shot);
		}

		PlayerAnimationState::State states[] = {
			PlayerAnimationState::State::GunFront,
			PlayerAnimationState::State::GunBack,
			PlayerAnimationState::State::GunRight,
			PlayerAnimationState::State::GunLeft,
		};

		vector<Vec3> directions = {
			transform->GetForward(),
			-transform->GetForward(),
			transform->GetRight(),
			-transform->GetRight(),
		};

		auto resultDirection = maru::Utility::CalculateNearDirect(velocity, directions);	//一番近い方向を取得

		//方向に合わせてEnumを取得
		PlayerAnimationState::State state;
		int index = 0;
		for (auto& direction : directions) {
			if (direction == resultDirection) {
				state = states[index];
				break;
			}
			index++;
		}

		if (animator->IsCurretAnimationState(state)) {	//同じアニメーションならfalse
			return;
		}

		animator->ChangePlayerAnimation(state);
	}

	void UseWepon::RotationUpdate() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto rotationController = m_rotationController.lock();
		if (!camera || !rotationController) {
			return;
		}

		if (m_isUseCamera) {
			m_direction = CalculateRotationDirection();
		}

		rotationController->SetDirect(m_direction);
	}

	void UseWepon::SettingReactiveIsAim() {
		auto& isAim = m_param.isAim;

		auto trueFunction = [&]() {		//Aim状態になった時
			auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
			if (status && status->IsDead()) {
				return;
			}

			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {	//アニメーションの遷移
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunSet2);
				m_soundEmitter.lock()->PlaySoundClip(m_readyArmsSoundClip);
			}
		};

		auto falseFunction = [&]() {	//Aim状態でなくなった時
			auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
			if (status && status->IsDead()) {
				return;
			}

			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunEnd2);
			}
		};

		isAim->AddFunction(true, trueFunction);
		isAim->AddFunction(false, falseFunction);
	}

	Vec3 UseWepon::CalculateRotationDirection() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto baseDirection = camera->GetAt() - camera->GetEye();
		auto selfTeamMember = GetGameObject()->GetComponent<I_TeamMember>(false);

		Vec3 resultVec = baseDirection;

		//playerの検索
		if (m_players.size() == 0) {
			SearchPlayers();	//playerを検索
		}

		auto cameraObject = camera->GetCameraObject();
		cameraObject->GetComponent<Transform>()->SetForward(baseDirection);
		auto eye = cameraObject->GetComponent<EyeSearchRange>(false);
		if (!eye) {
			eye = cameraObject->AddComponent<EyeSearchRange>();
			constexpr float Height = 20.0f;
			constexpr float Degree = 40.0f;
			eye->SetEyeHeight(Height);
			eye->SetEyeDegree(Degree);
		}

		//距離管理データ
		struct Data {
			std::weak_ptr<GameObject> object;
			Vec3 toTargetVec;

			Data(const std::shared_ptr<GameObject>& object, const Vec3& toTargetVec) :
				object(object),
				toTargetVec(toTargetVec)
			{}

			float GetToTargetLength() const { return toTargetVec.length(); }
		};

		//一番近い敵をターゲットにする。
		std::vector<Data> datas;
		for (auto& player : m_players) {
			if (eye->IsInEyeRange(player.lock())) {
				auto toTargetVec = maru::Utility::CalcuToTargetVec(GetGameObject(), player.lock());

				datas.push_back(Data(player.lock(), toTargetVec));
			}
		}

		if (datas.size() != 0) {
			std::sort(
				datas.begin(), datas.end(), 
				[](const Data left, const Data right) { return left.GetToTargetLength() < right.GetToTargetLength(); }
			);

			resultVec = datas[0].toTargetVec;
		}

		return resultVec;
	}

	void UseWepon::SearchPlayers() {
		auto selfTeamMember = GetGameObject()->GetComponent<I_TeamMember>(false);
		auto players = maru::Utility::FindGameObjects<PlayerObject>(GetStage());
		for (auto player : players) {
			auto teamMember = player->GetComponent<I_TeamMember>(false);
			if (!teamMember) {
				continue;
			}

			if (teamMember->GetTeam() != selfTeamMember->GetTeam()) {	//同じチームでないなら
				m_players.push_back(player);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void UseWepon::SetIsAim(const bool isAim) { 
		*m_param.isAim = isAim; 
	}

	bool UseWepon::IsAim() const { return m_param.isAim->GetValue(); }

	void UseWepon::SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept { m_wepon = wepon; }

	std::shared_ptr<WeponBase> UseWepon::GetWepon() const noexcept { return m_wepon.lock(); }

	float UseWepon::GetWeaponWeight() const noexcept {
		auto weapon = GetWepon();
		return weapon ? weapon->GetWeight() : 0.0f;	//武器があるなら武器の重さを返す。
	}

}