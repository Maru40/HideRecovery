/*!
@file LookTargetManager.cpp
@brief LookTargetManagerなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "LookTargetManager.h"

#include "MaruUtility.h"
#include "EnemyBase.h"
#include "EnemyObjectBase.h"

#include "PlayerInputer.h"

#include "ChangeCameraManager.h"

#include "Targeted.h"

namespace basecross {

	using Enemy::EnemyBase;
	using Enemy::EnemyObjectBase;

	//パラメータ----------------------------------------------------------------------------------

	LookTargetManager::Parametor::Parametor()
		:Parametor(10.0f, 30.0f)
	{}

	LookTargetManager::Parametor::Parametor(const float& inputChangeRange, const float& inputChangeDegree)
		:inputChangeRange(inputChangeRange), inputChangeDegree(inputChangeDegree)
	{}

	//--------------------------------------------------------------------------------------------

	LookTargetManager::LookTargetManager(const std::shared_ptr<GameObject>& objPtr)
		:LookTargetManager(objPtr, Parametor())
	{}

	LookTargetManager::LookTargetManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
		:Component(objPtr), m_param(parametor)
	{}

 	void LookTargetManager::OnUpdate() {
		if (!m_target) {
			return;
		}

		//ターゲットの切り替え管理。
		ChangeTargetManager();
	}

	void LookTargetManager::ChangeTargetManager() {
		auto input = PlayerInputer::GetRSticVec3();
		if (input == Vec3(0.0f)) {
			return;
		}

		//敵の近くの敵をサーチ。
		auto targets = SearchTargetNearTargets(m_param.inputChangeRange, m_target.GetShard());
		if (targets.empty()) {
			return;
		}

		//敵がその方向にいる場合は、そのターゲットを取得
		auto target = FindDirectTarget(GetTarget(), targets);
		if (target) {
			SetTarget(target);
			ChangeCameraManager::GetInstance()->ChangeState(ChangeCameraManager::State::Look);
		}
	}

	//ターゲット近くのターゲットを取得する。
	vector<std::shared_ptr<GameObject>> LookTargetManager::SearchTargetNearTargets(
		const float& range, const std::shared_ptr<GameObject>& target)
	{
		vector<std::shared_ptr<GameObject>> resultObjects;

		auto enemys = maru::Utility::FindComponents<Targeted>();
		for (const auto& enemy : enemys) {
			if (!enemy->GetUpdateActive()) {  //Targetedがfalseなら処理を飛ばす。
				continue;
			}

			auto enemyObject = enemy->GetGameObject();
			if (GetTarget() == enemyObject) { //同じオブジェクトなら処理をしない。
				continue;
			}

			if (maru::Utility::IsInTargetCircleRange(target, enemyObject, range)) {
				resultObjects.push_back(enemyObject);
			}
		}

		return resultObjects;
	}

	std::shared_ptr<GameObject> LookTargetManager::FindDirectTarget(
		const std::shared_ptr<GameObject>& currentTarget, const std::vector<std::shared_ptr<GameObject>>& nearTargets) 
	{
		auto input = PlayerInputer::GetRSticVec3();
		if (input == Vec3(0.0f)) {
			return nullptr;
		}

		auto camera = GetStage()->GetView()->GetTargetCamera();
		input = maru::Utility::CalcuCameraVec(input, camera, GetGameObject());
		//角度差が範囲内のオブジェクトに絞る。
		auto targets = maru::Utility::FindNearDirectTargetsDegree(input, currentTarget, nearTargets, m_param.inputChangeDegree);
		//絞った中で一番近いターゲットを取得。
		auto target = maru::Utility::CalcuNearObject(targets, currentTarget->GetComponent<Transform>()->GetPosition());

		return target;
	}

	std::shared_ptr<GameObject> LookTargetManager::SearchTarget() {
		//自分の近くの敵を取得
		std::vector<std::shared_ptr<GameObject>> enemys;
		auto targets = maru::Utility::FindComponents<Targeted>();
		for (auto& target : targets) {
			if (target->GetUpdateActive()) {
				enemys.push_back(target->GetGameObject());
			}
		}
		auto nearObject = maru::Utility::CalcuNearObject(enemys, transform->GetPosition());
		SetTarget(nearObject);
		return nearObject;
	}
}