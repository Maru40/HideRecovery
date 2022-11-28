/*!
@file ObserveIsInEyeTarget.cpp
@brief ObserveIsInEyeTarget�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ObserveIsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	ObserveIsInEyeTarget::ObserveIsInEyeTarget(const std::shared_ptr<EyeSearchRange>& eyeRange):
		ObserveIsInEyeTarget(eyeRange, ObserveTargets())
	{}

	ObserveIsInEyeTarget::ObserveIsInEyeTarget(
		const std::shared_ptr<EyeSearchRange>& eyeRange,
		const ObserveSharedTargets& observeTargets
	):
		ObserveIsInEyeTarget(eyeRange, maru::Utility::ConvertArraySharedToWeak(observeTargets))
	{}

	ObserveIsInEyeTarget::ObserveIsInEyeTarget(
		const std::shared_ptr<EyeSearchRange>& eyeRange,
		const ObserveTargets& observeTargets
	):
		m_eyeRange(eyeRange),
		m_observeTargets(observeTargets)
	{}

	std::shared_ptr<GameObject> ObserveIsInEyeTarget::SearchIsInEyeTarget() const {
		auto eyeRange = m_eyeRange.lock();
		if (!eyeRange) {
			Debug::GetInstance()->Log(L"IsInEyeTarget::CanTransition() : �K�v�R���|�[�l���g������܂���B");
			return nullptr;	//���E���Ȃ����画�f�ł��Ȃ����߁A�Ď��ł��Ȃ��B
		}

		for (auto& weakTarget : m_observeTargets) {
			if (weakTarget.expired()) {
				continue;	//�Ώۂ��Ȃ��Ȃ�ΏۊO
			}

			//���E�͈͓��Ȃ�A�J�ڂł���B
			auto target = weakTarget.lock();
			auto targetPosition = target->GetComponent<Transform>()->GetPosition();
			if (eyeRange->IsInEyeRange(targetPosition)) {
				return target;
			}
		}

		return nullptr;
	}

	std::vector<std::shared_ptr<GameObject>> ObserveIsInEyeTarget::SearchIsInEyeTargets() const {
		std::vector<std::shared_ptr<GameObject>> result;

		auto eyeRange = m_eyeRange.lock();
		if (!eyeRange) {
			Debug::GetInstance()->Log(L"IsInEyeTarget::CanTransition() : �K�v�R���|�[�l���g������܂���B");
			return result;	//���E���Ȃ����画�f�ł��Ȃ����߁A�Ď��ł��Ȃ��B
		}

		for (auto& weakTarget : m_observeTargets) {
			if (weakTarget.expired()) {
				continue;	//�Ώۂ��Ȃ��Ȃ�ΏۊO
			}

			//���E�͈͓��Ȃ�A�J�ڂł���B
			auto target = weakTarget.lock();
			if (!target) {
				continue;
			}

			auto targetPosition = target->GetComponent<Transform>()->GetPosition();
			if (eyeRange->IsInEyeRange(targetPosition)) {
				result.push_back(target);
			}
		}

		return result;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void ObserveIsInEyeTarget::SetObserveTargets(const ObserveSharedTargets& targets) {
		SetObserveTargets(maru::Utility::ConvertArraySharedToWeak(targets));
	}

}