
/*!
@file WallAvoid.cpp
@brief WallAvoid�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "WallAvoid.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�ǉ��p�̐G�o
	//--------------------------------------------------------------------------------------

	WallAvoidTacticle::WallAvoidTacticle():
		WallAvoidTacticle(0.0f)
	{}

	WallAvoidTacticle::WallAvoidTacticle(const float degree):
		WallAvoidTacticle(1.0f, degree)
	{}

	WallAvoidTacticle::WallAvoidTacticle(const float range, const float degree):
		m_range(range),
		m_degree(degree)
	{}

	//--------------------------------------------------------------------------------------
	///	�ǉ���p�����[�^
	//--------------------------------------------------------------------------------------

	WallAvoid_Parametor::WallAvoid_Parametor():
		avoidPower(0.75f),
		offset(Vec3(0.0f, -0.35f, 0.0f))
	{}

	//--------------------------------------------------------------------------------------
	///	�ǉ��
	//--------------------------------------------------------------------------------------

	WallAvoid::WallAvoid(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr),
		m_param(Parametor()),
		m_avoidVec(Vec3(0.0f))
	{}

	void WallAvoid::OnCreate() {

	}

	void WallAvoid::OnStart() {
		m_transform = GetGameObject()->GetComponent<Transform>();
	}

	void WallAvoid::OnLateStart() {
		//��Q���ݒ�
		SettingDefaultObstacles();

		//�G�o�̐���
		SettingDefaultTacticles();
	}

	void WallAvoid::OnUpdate() {
		AvoidUpdate();
	}

	void WallAvoid::AvoidUpdate() {
		for (auto tacticle : m_tacticles) {
			auto avoidVec = CalculateAvoidVec(tacticle);
			if (avoidVec != Vec3(0.0f)) {
				m_avoidVec = avoidVec;	//���͂ɗ͂�������B
				break;
			}
		}
	}

	Vec3 WallAvoid::CalculateAvoidVec(const std::shared_ptr<WallAvoidTacticle>& tacticle) {
		auto result = Vec3(0.0f);
		Vec3 startPosition = m_transform.lock()->GetPosition() + m_param.offset;
		auto rayDirection = CalculateRayDirection(tacticle);
		Vec3 targetPosition = startPosition + rayDirection;

		//���C���q�b�g������A����������Ԃ��B
		if (maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, m_obstacles)) {
			result += -rayDirection;
		}

		return result.GetNormalized() * m_param.avoidPower;
	}

	Vec3 WallAvoid::CalculateRayDirection(const std::shared_ptr<WallAvoidTacticle>& tacticle) {
		auto forward = m_transform.lock()->GetForward();
		auto direction = maru::Mathf::RotationDegreeVec(forward, tacticle->GetDegree(), Vec3::Up());
		return direction.GetNormalized() * tacticle->GetRange();
	}

	void WallAvoid::SettingDefaultObstacles() {
		//�V�F�A�N���X�Ǘ������݂��Ȃ��Ȃ�A���������Ȃ��B
		if (!ShareClassesManager::GetInstance()) {
			return;
		}

		auto objects = GetStage()->GetGameObjectVec();
		//auto players = maru::Utility::FindGameObjects<PlayerObject>();
		//std::vector<std::shared_ptr<GameObject>> excludes;
		//for (auto& player : players) {
		//	excludes.push_back(player);
		//}
		auto obstacles = maru::UtilityObstacle::FindObstacles(objects, maru::UtilityObstacle::DEFAULT_OBSTACLE_AND_PLAYER_AND_HIDEPLACE);
		//auto obs = maru::Utility::ExcludeVector(obstacles, excludes);

		m_obstacles = maru::Utility::ConvertArraySharedToWeak(obstacles);
	}

	void WallAvoid::SettingDefaultTacticles() {
		constexpr float DEGREE_VALUE = 15.0f;
		constexpr float DEGREE_VALUE_Side =40.0f;
		float degrees[] = {
			+DEGREE_VALUE,
			-DEGREE_VALUE,
			+DEGREE_VALUE_Side,
			-DEGREE_VALUE_Side
		};

		//�f�t�H�œ�̐G�o���͂₷�B
		for (float degree : degrees) {
			auto tacticle = std::make_shared<WallAvoidTacticle>(degree);
			m_tacticles.push_back(tacticle);
		}
	}

}