#include "AIActiveChanger.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Maruyama/StateMachine/StateMachine.h"
#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Itabashi/ObjectMover.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross
{
	AIActiveChanger::AIActiveChanger(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void AIActiveChanger::OnLateStart()
	{
		auto ownerObject = GetGameObject();

		m_seekTarget = ownerObject->GetComponent<SeekTarget>();
		m_selfAStarNodeController = ownerObject->GetComponent<SelfAstarNodeController>();
		m_targetManager = ownerObject->GetComponent<TargetManager>();
		m_aiVertualController = ownerObject->GetComponent<AIVirtualController>();
		m_enemyBase = ownerObject->GetComponent<Enemy::EnemyBase>();
		m_aiPlayerStator = ownerObject->GetComponent<Enemy::AIPlayerStator>();
	}

	void AIActiveChanger::AIActiveChange(bool isActive)
	{
		if (isActive == false) {
			return;
		}

		Debug::GetInstance()->Log(L"▲▲AIChange▲▲");

		m_seekTarget.lock()->SetUpdateActive(isActive);
		m_selfAStarNodeController.lock()->SetUpdateActive(isActive);
		m_targetManager.lock()->SetUpdateActive(isActive);
		m_aiVertualController.lock()->SetUpdateActive(isActive);
		m_enemyBase.lock()->SetUpdateActive(isActive);
		m_aiPlayerStator.lock()->SetUpdateActive(isActive);

		if (auto objectMover = GetGameObject()->GetComponent<Operator::ObjectMover>(false)) {
			Debug::GetInstance()->Log(L"▲▲MoverFalse▲▲");
			objectMover->SetCameraAffected(false);
			objectMover->SetDefaultForward(Vec3::Forward());
		}

		if (auto useWeapon = GetGameObject()->GetComponent<UseWeapon>(false)) {
			Debug::GetInstance()->Log(L"▲▲UseWeaponFalse▲▲");
			useWeapon->SetIsUseCamera(false);
		}
	}
}