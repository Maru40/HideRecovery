/*!
@file EyeSearchRangeParam.cpp
@brief EyeSearchRangeParamなど実体
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
		//長さチェック
		return toVec.length() <= m_param.length ? true : false;
	}

	bool EyeSearchRange::IsHeight(const std::shared_ptr<GameObject>& target) {
		auto selfPos = transform->GetPosition();
		auto targetPos = target->GetComponent<Transform>()->GetPosition();

		auto subHeight = targetPos.y - selfPos.y;  //高さの差を求める。
		//高さが小さかったら。
		return std::abs(subHeight) <= m_param.height ? true : false;
	}

	bool EyeSearchRange::IsRad(const std::shared_ptr<GameObject>& target) {
		auto forward = transform->GetForward();
		forward.y = 0.0f;
		auto toVec = maru::Utility::CalcuToTargetVec(GetGameObject(), target);
		toVec.y = 0.0f;

		auto newDot = dot(forward.GetNormalized(), toVec.GetNormalized());
		auto newRad = acosf(newDot);
		//索敵範囲に入っていたら。
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
			if (IsInEyeRange(param.target)) {  //ターゲットが視界に入っていたら。
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

		//全ての条件がtrueなら視界内に対象がいる。
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

		bool isInEeyRange = IsInEyeRange(target);  //結果を保存
		m_param = defaultParam;

		return isInEeyRange;
	}


}

//endbasecross