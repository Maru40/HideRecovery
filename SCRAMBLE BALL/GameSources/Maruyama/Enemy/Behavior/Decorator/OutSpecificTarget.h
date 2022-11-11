/*!
@file OutSpecificTarget.h
@brief OutSpecificTargetなど
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
				/// 特定のターゲットの時だけ、遷移を除外する処理
				//--------------------------------------------------------------------------------------
				class OutSpecificTarget : public DecoratorBase<Enemy::EnemyBase> 
				{
					std::weak_ptr<GameObject> m_target;				//ターゲット

					std::weak_ptr<TargetManager> m_targetManager;	//ターゲット管理

				public:
					OutSpecificTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const std::shared_ptr<GameObject>& target
					);

					bool CanTransition() const override;

					bool CanUpdate() override;

				public:
					//--------------------------------------------------------------------------------------
					/// アクセッサ
					//--------------------------------------------------------------------------------------

					void SetTarget(const std::shared_ptr<GameObject>& target) { m_target = target; }

					_NODISCARD std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }
				};

			}
		}
	}
}