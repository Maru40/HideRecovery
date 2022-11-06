/*!
@file IsInEyeTargets.h
@brief IsInEyeTargetsなど
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
	class TargetManager;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータのパラメータ
				//--------------------------------------------------------------------------------------
				struct IsInEyeTargets_Parametor {
					float lostIntervalTime;	//見失った後に追いかける時間
					float farRange;			//追従不可能な程遠くに行ったと判断する距離

					IsInEyeTargets_Parametor();

					IsInEyeTargets_Parametor(const float lostIntervalTime, const float farRange);
				};

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータ
				//--------------------------------------------------------------------------------------
				class IsInEyeTargets : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
					using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

					using Parametor = IsInEyeTargets_Parametor;

				private:
					Parametor m_param;	//パラメータ

					std::unique_ptr<GameTimer> m_timer;							//時間管理
					std::unique_ptr<ObserveIsInEyeTarget> m_observeIsInTarget;	//監視処理担当クラス。

					std::weak_ptr<EyeSearchRange> m_eyeRange;					//視界管理クラス
					std::weak_ptr<TargetManager> m_targetManager;				//目標監視クラス
					
				public:
					IsInEyeTargets(const std::shared_ptr<Enemy::EnemyBase>& owner);

					IsInEyeTargets(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveSharedTargets& observeTargets,
						const Parametor& parametor = Parametor()
					);

					IsInEyeTargets(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveTargets& observeTargets,
						const Parametor& parametor = Parametor()
					);

					virtual ~IsInEyeTargets() = default;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					bool IsLost() const;

					/// <summary>
					/// 目標が遠くにいるかどうか
					/// </summary>
					/// <returns>遠くにいるならtrue</returns>
					bool IsFarRange() const;

				public:

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

					void SetLostIntervalTime(const float time) noexcept { m_param.lostIntervalTime = time; };

					_NODISCARD float GetLostIntervalTime() const noexcept { return m_param.lostIntervalTime; }
				};

			}
		}
	}
}