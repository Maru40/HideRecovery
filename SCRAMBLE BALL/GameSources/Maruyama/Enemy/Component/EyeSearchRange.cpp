/*!
@file EyeSearchRangeParam.cpp
@brief EyeSearchRangeParam�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "EyeSearchRange.h"
#include "Maruyama/Utility/Utility.h"

#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	EyeSearchRange::EyeSearchRange(const std::shared_ptr<GameObject>& objPtr)
		:EyeSearchRange(objPtr, EyeSearchRangeParametor())
	{}

	EyeSearchRange::EyeSearchRange(const std::shared_ptr<GameObject>& objPtr,
		const EyeSearchRangeParametor& param
	):
		Component(objPtr),
		m_param(param)
	{}


	bool EyeSearchRange::IsRange(const std::shared_ptr<GameObject>& target) {
		return IsRange(target->GetComponent<Transform>()->GetPosition());
	}

	bool EyeSearchRange::IsRange(const Vec3& targetPosition) {
		auto toVec = targetPosition - transform->GetPosition();
		//�����`�F�b�N
		return toVec.length() <= m_param.length ? true : false;
	}

	bool EyeSearchRange::IsHeight(const std::shared_ptr<GameObject>& target) {
		return IsHeight(target->GetComponent<Transform>()->GetPosition());
	}

	bool EyeSearchRange::IsHeight(const Vec3& targetPosition) {
		auto selfPosition = transform->GetPosition();

		auto subHeight = targetPosition.y - selfPosition.y;  //�����̍������߂�B
		//������������������B
		return std::abs(subHeight) <= m_param.height ? true : false;
	}

	bool EyeSearchRange::IsRad(const std::shared_ptr<GameObject>& target) {
		return IsRad(target->GetComponent<Transform>()->GetPosition());
	}

	bool EyeSearchRange::IsRad(const Vec3& targetPosition) {
		auto forward = transform->GetForward();
		forward.y = 0.0f;
		auto toVec = targetPosition - transform->GetPosition();
		toVec.y = 0.0f;

		auto newDot = dot(forward.GetNormalized(), toVec.GetNormalized());
		auto newRad = acosf(newDot);
		//���G�͈͂ɓ����Ă�����B
		return newRad <= m_param.rad ? true : false;
	}

	bool EyeSearchRange::IsRay(const std::shared_ptr<GameObject>& target){
		return IsRay(target->GetComponent<Transform>()->GetPosition());
	}

	bool EyeSearchRange::IsRay(const Vec3& targetPosition) {
		return !maru::UtilityObstacle::IsRayObstacle(transform->GetPosition(), targetPosition, GetStage()->GetGameObjectVec());
	}

	void EyeSearchRange::Hit(const EyeTargetParametor& targetParam) {
		targetParam.isFind = true;
	}

	void EyeSearchRange::OnUpdate() {
		for (auto& param : m_targetParams) {
			if (IsInEyeRange(param.target)) {  //�^�[�Q�b�g�����E�ɓ����Ă�����B
				Hit(param);
			}
			else {
				param.isFind = false;
			}
		}
	}

	bool EyeSearchRange::IsInEyeRange(std::shared_ptr<GameObject>& target) {
		if (target == nullptr) {
			return false;
		}

		auto targetPosition = target->GetComponent<Transform>()->GetPosition();
		return IsInEyeRange(targetPosition);
	}

	bool EyeSearchRange::IsInEyeRange(const Vec3& position) {
		//�S�Ă̏�����true�Ȃ王�E���ɑΏۂ�����B
		if (IsRange(position) && IsHeight(position) && IsRad(position) && IsRay(position)) {
			return true;
		}
		else {
			return false;
		}
	}

	bool EyeSearchRange::IsInEyeRange(std::shared_ptr<GameObject>& target, const float& length) {
		auto defaultParam = m_param;
		m_param.length = length;

		bool isInEeyRange = IsInEyeRange(target);  //���ʂ�ۑ�
		m_param = defaultParam;

		return isInEeyRange;
	}

	bool EyeSearchRange::IsInEyeRange(std::shared_ptr<GameObject>& target, const Parametor& param) {
		auto defaultParam = m_param;
		m_param = param;

		bool isInEeyRange = IsInEyeRange(target);  //���ʂ�ۑ�
		m_param = defaultParam;

		return isInEeyRange;
	}


}

//endbasecross