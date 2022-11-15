
/*!
@file CombatCalculater.cpp
@brief CombatCalculater�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCalculater.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/Targeted.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {


			float Combat::CalculateEvalutionValue(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& target
			) {
				return CalculateEvalutionValue(selfObject->GetComponent<Transform>(), target);
			}

			float Combat::CalculateEvalutionValue(
				const std::shared_ptr<Transform>& selfTransform,
				const std::shared_ptr<GameObject>& target
			) {
				float result = 0.0f;

				auto targetTrans = target->GetComponent<Transform>();

				float toTargetRange = (targetTrans->GetPosition() - selfTransform->GetPosition()).length();
				result += toTargetRange;	//������]���l�Ƃ���B

				//�^�[�Q�b�g�{�̗̂D��x��ǉ�
				if (auto targeted = target->GetComponent<Targeted>(false)) {
					result *= targeted->GetPriority();
				}

				//player�X�e�[�^�X������ꍇ
				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hp���Ⴂ�قǁA����������(�^�[�Q�b�g�ɂ��₷������B)
				}

				return result;
			}

		}
	}
}