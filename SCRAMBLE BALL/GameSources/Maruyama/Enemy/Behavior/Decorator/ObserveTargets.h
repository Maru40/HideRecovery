/*!
@file ObserveTargets.h
@brief ObserveTargets�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class TargetManager;
	class GameTimer;

	class ObserveIsInEyeTarget;
	class I_TeamMember;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				class ObserveTargets : public DecoratorBase<Enemy::EnemyBase> 
				{
					std::weak_ptr<Transform> m_transform;
					std::weak_ptr<I_TeamMember> m_teamMember;
					std::weak_ptr<TargetManager> m_targetManager;

					std::unique_ptr<ObserveIsInEyeTarget> m_observeEyeTargets;

				public:
					ObserveTargets(
						const std::shared_ptr<Enemy::EnemyBase>& owner, 
						const std::vector<std::shared_ptr<GameObject>>& targets
					);

					virtual ~ObserveTargets() = default;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override;

				private:
					/// <summary>
					/// �^�[�Q�b�g�𔭌��������Ƃ�`����B
					/// </summary>
					/// <param name="targets">�^�[�Q�b�g�z��</param>
					void NotifyTargets(const std::vector<std::shared_ptr<GameObject>>& targets);

					/// <summary>
					/// �^�[�Q�b�g�Ǘ��Ŏw�肳��Ă���^�[�Q�b�g�ȊO�ɂ��ĕԂ��B
					/// </summary>
					/// <param name="targets">�^�[�Q�b�g���z��</param>
					/// <returns>���łɃ^�[�Q�b�g�ɂ��Ă��鑊��ȊO</returns>
					std::vector<std::shared_ptr<GameObject>> SearchOtherTarget(const std::vector<std::shared_ptr<GameObject>>& targets) const;

					bool HasTarget() const;

					std::vector<std::shared_ptr<GameObject>> SearchObserveTargets() const;
				};

			}
		}
	}
}