
/*!
@file CombatCalculater.cpp
@brief CombatCalculater�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolCalculater.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			float HidePlacePatorl::CalculateEvalutionValue(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& target
			) {
				float result = 0.0f;

				float toTargetRange = maru::Utility::CalcuToTargetVec(selfObject, target).length();
				result += toTargetRange;	//������]���l�Ƃ���B

				//�^�[�Q�b�g�{�̗̂D��x��ǉ�
				if (auto targeted = target->GetComponent<Targeted>(false)) {
					result *= targeted->GetPriority();
				}

				//�����hp�ɂ���ėD��x��������B
				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hp���Ⴂ�قǁA����������(�^�[�Q�b�g�ɂ��₷������B)
				}

				return result;
			}

		}
	}
}