
/*!
@file Utility_EnemyStator.cpp
@brief Utility_EnemyStator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Utility_EnemyStator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/Enemy/Astar/GraphBase.h"
#include "Maruyama/Enemy/StateMachine/StateNodeBase.h"
#include "Maruyama/Enemy/StateMachine/StateEdgeBase.h"
#include "Maruyama/Enemy/Component/Animation/MaruAnimationHelper.h"
#include "Maruyama/Enemy/Component/Animation/AnimationMainStateMachine.h"

#include "GameStageBase.h"

#include "Maruyama/DebugClass/Object/DebugObject.h"

namespace basecross {
	namespace Enemy {
		namespace UtilityStator {

			//�v���C���[�����E�ɓ��������̑J�ڔ���
			bool Transition::InEyeRangePlayer(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				if (!eye || !targetManager) {
					return false;
				}

				std::shared_ptr<GameObject> target = selfObject->GetGameStage()->GetPlayer();

				if (eye->IsInEyeRange(target, eyeRange)) {
					targetManager->SetTarget(target);
					return true;
				}

				return false;
			}

			//�^�[�Q�b�g�����E�ɓ��������̑J�ڔ���
			bool Transition::InEyeRangeTarget(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				if (!eye || !targetManager) {
					return false;
				}

				auto target = targetManager->GetTarget();
				return eye->IsInEyeRange(target, eyeRange);
			}

			//�^�[�Q�b�g�����X�g�����J�ڔ���
			bool Transition::LostTarget(const std::shared_ptr<GameObject>& selfObject, const float& maxChaseRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				//�~�����R���|�[�l���g������������
				if (!eye || !targetManager) {
					DebugObject::AddString(L"LostTarget�ŁA�J�ڂ��邽�߂̃R���|�[�l���g������܂���B");
					return false;
				}

				//�^�[�Q�b�g��Null�Ȃ�J�ڂ���B
				if (!targetManager->HasTarget()) {
					return true;
				}

				//�͈͊O��������Ǐ]����߂�
				auto toTargetVec = targetManager->CalcuToTargetVec();
				if (toTargetVec.length() > maxChaseRange) {
					targetManager->SetTarget(nullptr);
					return true;
				}

				return false;
			}

			bool Transition::ComparisonVelocity(const std::shared_ptr<GameObject>& selfObject,
				const float& transitionVelocity, 
				const ComparisonType& type)
			{
				auto velocityManager = selfObject->GetComponent<VelocityManager>(false);
				if (!velocityManager) {
					return true;
				}

				switch (type)
				{
				case ComparisonType::Less:
					return velocityManager->GetWorldVelocity().length() < transitionVelocity;
					break;

				case ComparisonType::OrLess:
					return velocityManager->GetWorldVelocity().length() <= transitionVelocity;
					break;

				case ComparisonType::Greater:
					return velocityManager->GetWorldVelocity().length() > transitionVelocity;
					break;

				case ComparisonType::OrGreater:
					return velocityManager->GetWorldVelocity().length() >= transitionVelocity;
					break;
				}

				return true;
			}
		}
	}
}