/*!
@file EyeSearchRangeParam.cpp
@brief EyeSearchRangeParam�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "EyeSearchRange.h"
#include "MaruUtility.h"
#include "DebugObject.h"

#include "UtilityObstacle.h"

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
		auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), target);
		//�����`�F�b�N
		return toVec.length() <= m_param.length ? true : false;
	}

	bool EyeSearchRange::IsHeight(const std::shared_ptr<GameObject>& target) {
		auto selfPos = transform->GetPosition();
		auto targetPos = target->GetComponent<Transform>()->GetPosition();

		auto subHeight = targetPos.y - selfPos.y;  //�����̍������߂�B
		//������������������B
		return std::abs(subHeight) <= m_param.height ? true : false;
	}

	bool EyeSearchRange::IsRad(const std::shared_ptr<GameObject>& target) {
		auto forward = transform->GetForward();
		forward.y = 0.0f;
		auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), target);
		toVec.y = 0.0f;

		auto newDot = dot(forward.GetNormalized(), toVec.GetNormalized());
		auto newRad = acosf(newDot);
		//���G�͈͂ɓ����Ă�����B
		return newRad <= m_param.rad ? true : false;
	}

	bool EyeSearchRange::IsRay(const std::shared_ptr<GameObject>& target){
		auto targetPosition = target->GetComponent<Transform>()->GetPosition();
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

		//�S�Ă̏�����true�Ȃ王�E���ɑΏۂ�����B
		if (IsRange(target) && IsHeight(target) && IsRad(target) && IsRay(target)) {
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


}

//endbasecross