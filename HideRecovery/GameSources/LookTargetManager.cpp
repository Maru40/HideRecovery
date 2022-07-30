/*!
@file LookTargetManager.cpp
@brief LookTargetManager�Ȃǎ���
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

	//�p�����[�^----------------------------------------------------------------------------------

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

		//�^�[�Q�b�g�̐؂�ւ��Ǘ��B
		ChangeTargetManager();
	}

	void LookTargetManager::ChangeTargetManager() {
		auto input = PlayerInputer::GetRSticVec3();
		if (input == Vec3(0.0f)) {
			return;
		}

		//�G�̋߂��̓G���T�[�`�B
		auto targets = SearchTargetNearTargets(m_param.inputChangeRange, m_target.GetShard());
		if (targets.empty()) {
			return;
		}

		//�G�����̕����ɂ���ꍇ�́A���̃^�[�Q�b�g���擾
		auto target = FindDirectTarget(GetTarget(), targets);
		if (target) {
			SetTarget(target);
			ChangeCameraManager::GetInstance()->ChangeState(ChangeCameraManager::State::Look);
		}
	}

	//�^�[�Q�b�g�߂��̃^�[�Q�b�g���擾����B
	vector<std::shared_ptr<GameObject>> LookTargetManager::SearchTargetNearTargets(
		const float& range, const std::shared_ptr<GameObject>& target)
	{
		vector<std::shared_ptr<GameObject>> resultObjects;

		auto enemys = maru::Utility::FindComponents<Targeted>();
		for (const auto& enemy : enemys) {
			if (!enemy->GetUpdateActive()) {  //Targeted��false�Ȃ珈�����΂��B
				continue;
			}

			auto enemyObject = enemy->GetGameObject();
			if (GetTarget() == enemyObject) { //�����I�u�W�F�N�g�Ȃ珈�������Ȃ��B
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
		//�p�x�����͈͓��̃I�u�W�F�N�g�ɍi��B
		auto targets = maru::Utility::FindNearDirectTargetsDegree(input, currentTarget, nearTargets, m_param.inputChangeDegree);
		//�i�������ň�ԋ߂��^�[�Q�b�g���擾�B
		auto target = maru::Utility::CalcuNearObject(targets, currentTarget->GetComponent<Transform>()->GetPosition());

		return target;
	}

	std::shared_ptr<GameObject> LookTargetManager::SearchTarget() {
		//�����̋߂��̓G���擾
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