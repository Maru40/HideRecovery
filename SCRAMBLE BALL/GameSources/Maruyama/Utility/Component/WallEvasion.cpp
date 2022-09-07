/*!
@file WallEvasion.cpp
@brief WallEvasion�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "WallEvasion.h"
#include "Maruyama/Utility/Utility.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/UtilityVelocity.h"
#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�\�[�g�p�֐�
	//--------------------------------------------------------------------------------------

	bool SortObstacleRange(const ObstacleRangePair& left, const ObstacleRangePair& right) {
		return left.range < right.range ? true : false;
	}

	//--------------------------------------------------------------------------------------
	///	��Q�����{��
	//--------------------------------------------------------------------------------------

	void WallEvasion::OnDestroy() {
		for (auto& object : m_tactileObjects) {
			GetStage()->RemoveGameObject<GameObject>(object.GetShard());
		}
	}

	std::vector<ObstacleRangePair> WallEvasion::CalucWallRangeSort() {
		vector<ObstacleRangePair> rangePairs;

		for (auto& obj : m_obstacleObjs) {
			auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), obj.GetShard());
			rangePairs.push_back(ObstacleRangePair(obj.GetShard(), toVec.length()));
		}

		std::sort(rangePairs.begin(), rangePairs.end(), &SortObstacleRange);

		return rangePairs;
	}

	Vec3 WallEvasion::CalucForce(const std::shared_ptr<GameObject>& tactile) {
		Vec3 returnVec(0.0f);

		if (m_obstacleObjs.size() == 0.0f) {
			return returnVec;
		}

		auto obstacleRangePairs = CalucWallRangeSort();
		auto startPos = transform->GetPosition();
		for (auto& pair : obstacleRangePairs) {
			auto& obj = pair.object;
			auto col = obj->GetComponent<Collision>(false);
			if (!col) { //�R���W���������������珈�������Ȃ�
				continue;
			}

			RayHitData data;
			auto tactileTrans = tactile->GetComponent<Transform>();
			
			if (col->IsRayHit(startPos, tactileTrans->GetForward(), data)) {
				returnVec += startPos - data.point;
				break;
			}
		}

		returnVec.y = 0;
		return returnVec;
	}

	void WallEvasion::EvasionUpdate(const std::shared_ptr<TactileCtrl>& tactile, const std::shared_ptr<GameObject>& other) {
		if (GetUpdateActive() == false) {
			return;
		}

		auto delta = App::GetApp()->GetElapsedTime();
		auto newForce = CalucForce(tactile->GetGameObject());

		auto velocityComp = GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocityComp) {
			auto velocity = velocityComp->GetVelocity();
			
			auto power = m_maxSpeed - newForce.length();
			auto force = newForce.normalize() * power * delta;

			velocityComp->AddForce(force);
		}
	}

	//--------------------------------------------------------------------------------------
	///	�A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void WallEvasion::SetTactile(const std::shared_ptr<TactileCtrl>& tactile) {
		tactile->AddExcuteAction(
			[this](const std::shared_ptr<TactileCtrl>& tactile, const std::shared_ptr<GameObject>& other) {
			EvasionUpdate(tactile, other);
		});

		//�G�o�I�u�W�F�N�g�̓o�^
		m_tactileObjects.push_back(tactile->GetGameObject());
	}

}

//endbasecross