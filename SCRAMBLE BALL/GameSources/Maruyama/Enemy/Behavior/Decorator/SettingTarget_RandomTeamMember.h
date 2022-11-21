/*!
@file SettingTarget_HasBallOtherMember.h
@brief SettingTarget_HasBallOtherMemberなど
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

	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ランダムなチームメンバーをターゲットに指定する。
				//--------------------------------------------------------------------------------------
				class SettingTarget_RandomTeamMember : public DecoratorBase<Enemy::EnemyBase>
				{
					std::weak_ptr<TargetManager> m_targetManager;

				public:
					SettingTarget_RandomTeamMember(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }

				private:
					std::shared_ptr<GameObject> SearchTarget() const;
				};

			}
		}
	}
}