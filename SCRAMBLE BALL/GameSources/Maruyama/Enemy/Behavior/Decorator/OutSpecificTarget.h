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

				//--------------------------------------------------------------------------------------
				/// 特定のターゲットの時だけ、遷移を除外する処理
				//--------------------------------------------------------------------------------------
				class OutSpecificTarget : public DecoratorBase<Enemy::EnemyBase> 
				{
				public:
					OutSpecificTarget(const std::shared_ptr<Enemy::EnemyBase>& owner);

					bool CanTransition() const override;

					bool CanUpdate() override;
				};

			}
		}
	}
}