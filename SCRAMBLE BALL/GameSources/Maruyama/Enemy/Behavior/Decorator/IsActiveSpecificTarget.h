/*!
@file IsActiveSpecificTarget.h
@brief IsActiveSpecificTargetなど
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
				/// 特定のターゲットの時がActive状態の時のみ遷移する。
				//--------------------------------------------------------------------------------------
				class IsAcitiveSpecificTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<GameObject> m_target;	//ターゲット

				public:
					IsAcitiveSpecificTarget(
						const std::shared_ptr<GameObject>& owner,
						const std::shared_ptr<GameObject>& target
					);

					bool CanTransition() const override;

					bool CanUpdate() override;

				};

			}
		}
	}
}