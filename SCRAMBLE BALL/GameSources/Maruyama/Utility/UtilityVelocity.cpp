
/*!
@file UtilityVelocity.cpp
@brief UtilityVelocity�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "UtilityVelocity.h"

#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	Vec3 UtilityVelocity::MaxSpeedVecCheck(const Vec3 velocity, const float& maxSpeed) {
		auto speed = velocity.length();

		speed = maru::Mathf::Min(speed, maxSpeed);
		return normalize(velocity) * speed;
	}

	Vec3 UtilityVelocity::CalucSeekVec(const Vec3& velocity, const Vec3& toVec, const float& maxSpeed, const float forceMultipier) {
		Vec3 desiredVelocity = normalize(toVec) * maxSpeed;  //��]�̃x�N�g��
		return (desiredVelocity - velocity) * forceMultipier;
	}

	Vec3 UtilityVelocity::CalucArriveVec(const Vec3& velocity, const Vec3& toVec, const float& maxSpeed, const float forceMultipier, const float& decl) {
		float dist = toVec.length();
		if (dist > 0) {
			constexpr float DecelerationTweaker = 0.3f;  //�����l
			//�w�肳�ꂽ�����ŖڕW�ɓ��B���鎮
			float speed = dist / (decl * DecelerationTweaker); 
			speed = maru::Mathf::Min(speed,maxSpeed);
			Vec3 desiredVelocity = toVec * speed / dist; //��]�̃x�N�g��
			auto steerVec = desiredVelocity - velocity;  //�X�e�A�����O�x�N�g��
			return steerVec * forceMultipier;
		}
		return Vec3(0, 0, 0);
	}

	Vec3 UtilityVelocity::CalucNearArriveFarSeek(const Vec3& velocity, const Vec3& toVec,
		const float& maxSpeed, const float& nearRange, const float& decl)
	{
		auto range = toVec.length();
		if (range <= nearRange) {  //�߂��ɂ�����
			return CalucArriveVec(velocity, toVec, maxSpeed, decl);
		}
		else {  //�����ɂ�����
			return CalucSeekVec(velocity, toVec, maxSpeed);
		}
	}

	Vec3 UtilityVelocity::Reflection(const Vec3& moveDirect, const Vec3& normal)
	{
		auto direct = moveDirect;
		auto newDot = abs(dot(direct, normal));
		direct += 2.0f * (newDot * normal);

		return direct;
	}
}

//endbasecross