/*!
@file IsInEyeTarget.h
@brief IsInEyeTargetなど
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
	class ObserveIsInEyeTarget;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータ
				//--------------------------------------------------------------------------------------
				class IsInEyeTarget : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
					using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

				private:

					std::unique_ptr<ObserveIsInEyeTarget> m_observeIsInTarget;	//監視処理担当クラス。
					
				public:
					IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner);

					IsInEyeTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveSharedTargets& observeTargets
					);

					IsInEyeTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveTargets& observeTargets
					);

					virtual ~IsInEyeTarget() = default;

					bool CanTransition() const override;

					bool CanUpdate() override;

					//--------------------------------------------------------------------------------------
					/// アクセッサ
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// 監視対象の追加
					/// </summary>
					/// <param name="target">監視対象</param>
					void AddObserveTarget(const std::shared_ptr<GameObject>& target);

					void SetObserveTargets(const ObserveTargets& targets);

					void SetObserveTargets(const ObserveSharedTargets& targets);

					_NODISCARD ObserveTargets GetObserveTargets() const noexcept;

				};

			}
		}
	}
}