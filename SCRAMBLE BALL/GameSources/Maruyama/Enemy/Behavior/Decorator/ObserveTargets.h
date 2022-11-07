/*!
@file ObserveTargets.h
@brief ObserveTargetsなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
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
					/// ターゲットを発見したことを伝える。
					/// </summary>
					/// <param name="targets">ターゲット配列</param>
					void NotifyTargets(const std::vector<std::shared_ptr<GameObject>>& targets);

					/// <summary>
					/// ターゲット管理で指定されているターゲット以外にして返す。
					/// </summary>
					/// <param name="targets">ターゲット候補配列</param>
					/// <returns>すでにターゲットにしてある相手以外</returns>
					std::vector<std::shared_ptr<GameObject>> SearchOtherTarget(const std::vector<std::shared_ptr<GameObject>>& targets) const;

					bool HasTarget() const;

					std::vector<std::shared_ptr<GameObject>> SearchObserveTargets() const;
				};

			}
		}
	}
}