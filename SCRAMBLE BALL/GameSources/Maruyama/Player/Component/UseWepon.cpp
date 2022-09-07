
/*!
@file UseWepon.cpp
@brief UseWeponクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "UseWepon.h"

#include "WeponBase.h"

#include "Maruyama/Utility/ReactiveBool.h"

#include "Patch/PlayerInputer.h"

#include "Itabashi/ObjectMover.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "Maruyama/Utility/Component/RotationController.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Itabashi/GamePlayerManager.h"

#include "Maruyama/Player/Component/Teleport.h"
#include "Patch/SpringArmComponent.h"

#include "Maruyama/Load/StageMapCSV.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラスのパラメータ
	//--------------------------------------------------------------------------------------

	UseWepon_Parametor::UseWepon_Parametor() :
		UseWepon_Parametor(false, false)
	{}

	UseWepon_Parametor::UseWepon_Parametor(const bool isAim, const bool canShot) :
		defaultCameraSpeed(0.0f),
		aimCameraSpeed(0.0f),
		assitPower(3.0f),
		isAim(new maru::ReactiveBool(isAim)),
		canShot(new maru::ReactiveBool(canShot))
	{}

	UseWepon_Parametor::UseWepon_Parametor(const UseWepon_Parametor& parametor) :
		UseWepon_Parametor(
			parametor.isAim->GetValue(),
			parametor.canShot->GetValue()
		)
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

	void UseWepon::OnCreate() {
		constexpr int AimCameraSpeedIndex = 5;
		auto aimSpeedStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", AimCameraSpeedIndex);
		auto aimSpeed = static_cast<float>(_wtof(aimSpeedStr.c_str()));
		m_param.aimCameraSpeed = aimSpeed;


		constexpr int AssitPowerIndex = 8;
		auto assistPowerStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", AssitPowerIndex);
		auto assistPower = static_cast<float>(_wtof(assistPowerStr.c_str()));
		m_param.assitPower = assistPower;
	}

	void UseWepon::OnLateStart() {
		auto player = dynamic_pointer_cast<PlayerObject>(GetGameObject());
		if (player) {
			auto springArm = player->GetArm()->GetComponent<SpringArmComponent>(false);
			m_param.defaultCameraSpeed = springArm->GetSpeedXZ();
		}

		m_rotationController = GetGameObject()->GetComponent<RotationController>(false);
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		m_animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>(false);

		SettingReactiveIsAim();

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {	//アニメーションの遷移
			animator->AddAnimationEvent(
				PlayerAnimationState::State::GunSet2,
				nullptr,
				nullptr,
				[&]() { SetCanShot(true); }
			);
		}

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {	//アニメーションの遷移
			animator->AddAnimationEvent(
				PlayerAnimationState::State::GunEnd2,
				[&]() { SetCanShot(false); },
				nullptr,
				nullptr
			);
		}
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
			AssistCameraRotation(CalculateRotationDirection());
			auto baseDirection = camera->GetAt() - camera->GetEye();
			baseDirection.y = 0;
			m_direction = baseDirection;
		}

		rotationController->SetDirect(m_direction);
	}

	void UseWepon::AssistCameraRotation(const Vec3& direction) {
		auto player = dynamic_pointer_cast<PlayerObject>(GetGameObject());
		if (!player) {
			return;
		}

		auto delta = App::GetApp()->GetElapsedTime();
		auto springArm = player->GetArm()->GetComponent<SpringArmComponent>();
		auto camera = springArm->GetChildObject();
		
		auto cameraForward = transform->GetPosition() - camera->GetComponent<Transform>()->GetPosition();
		cameraForward.y = 0;
		auto selfForward = direction;
		selfForward.y = 0;

		auto newDot = dot(cameraForward.GetNormalized(), selfForward.GetNormalized());
		auto newRad = acosf(newDot);
		auto newDegree = XMConvertToDegrees(newRad);
		auto newCross = cross(cameraForward.GetNormalized(), selfForward.GetNormalized());

		constexpr float AssistDegree = 1.0f;
		if (newDegree <= AssistDegree) {	//アシストしない角度
			return;
		}

		auto rad = springArm->GetRadXZ();

		rad += -newCross.y * delta * m_param.assitPower;

		springArm->SetRadXZ(rad);
	}

	void UseWepon::SettingReactiveIsAim() {
		auto& isAim = m_param.isAim;

		//Aim状態になった時
		auto trueFunction = [&]() {		
			ChangeCameraSpeed(m_param.aimCameraSpeed);

			auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
			if (status && status->IsDead()) {
				return;
			}

			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {	//アニメーションの遷移
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunSet2);
				m_soundEmitter.lock()->PlaySoundClip(m_readyArmsSoundClip);
			}
		};

		//Aim状態でなくなった時
		auto falseFunction = [&]() {	
			ChangeCameraSpeed(m_param.defaultCameraSpeed);

			auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
			if (status && status->IsDead()) {
				return;
			}

			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {	//アニメーションの遷移
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GunEnd2);
			}
		};

		isAim->AddFunction(true, trueFunction);
		isAim->AddFunction(false, falseFunction);
	}

	void UseWepon::SettingReactiveCanShot() {
		auto& canShot = m_param.canShot;
	}

	Vec3 UseWepon::CalculateRotationDirection() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto baseDirection = camera->GetAt() - camera->GetEye();
		baseDirection.y = 0;
		//return baseDirection;
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
			eye->SetEyeHeight(Height);

			constexpr int AssistRangeIndex = 6;
			auto assistRangeStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", AssistRangeIndex);
			auto assistRange = static_cast<float>(_wtof(assistRangeStr.c_str()));
			eye->SetEyeLength(assistRange);

			constexpr int AssistDegreeIndex = 7;
			auto assistDegreeStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", AssistDegreeIndex);
			auto assistDegree = static_cast<float>(_wtof(assistDegreeStr.c_str()));
			eye->SetEyeDegree(assistDegree);
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
		auto eyeParam = eye->GetParametor();
		std::vector<Data> datas;
		for (auto& player : m_players) {
			auto status = player.lock()->GetComponent<PlayerStatus>(false);
			if (status && status->IsDead()) {
				continue;
			}

			if (eye->IsRange(player.lock()) && 
				eye->IsRad(baseDirection, transform->GetPosition(), player.lock()->GetComponent<Transform>()->GetPosition(), eyeParam.rad) &&
				eye->IsRay(player.lock())
			) {
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

	void UseWepon::ChangeCameraSpeed(const float speed) {
		auto player = dynamic_pointer_cast<PlayerObject>(GetGameObject());
		if (!player) {
			return;
		}

		auto springArm = player->GetArm()->GetComponent<SpringArmComponent>(false);
		if (!springArm) {
			return;
		}

		springArm->SetSpeedXZ(speed);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void UseWepon::SetIsAim(const bool isAim) { *m_param.isAim = isAim; }

	bool UseWepon::IsAim() const { return m_param.isAim->GetValue(); }

	void UseWepon::SetCanShot(const bool isShot) { *m_param.canShot = isShot; }

	bool UseWepon::CanShot() const { return m_param.canShot->GetValue(); }

	void UseWepon::SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept { m_wepon = wepon; }

	std::shared_ptr<WeponBase> UseWepon::GetWepon() const noexcept { return m_wepon.lock(); }

	float UseWepon::GetWeaponWeight() const noexcept {
		auto weapon = GetWepon();
		return weapon ? weapon->GetWeight() : 0.0f;	//武器があるなら武器の重さを返す。
	}

}