/*!
@file IsInEyeTarget.h
@brief IsInEyeTargetなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class TargetManager;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				enum class IsInEyeTarget_CanType {
					In,
					Out
				};

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータのパラメータ
				//--------------------------------------------------------------------------------------
				struct IsInEyeTarget_Parametor {
					EyeSearchRangeParametor eyeParametor;	//視界パラメータ
					float minLostIntervalTime;				//見失った後に追いかける時間(最小)
					float maxLostIntervalTime;				//見失った後に追いかける時間(最大)
					float farRange;							//追従不可能な程遠くに行ったと判断する距離

					IsInEyeTarget_Parametor();

					IsInEyeTarget_Parametor(const EyeSearchRangeParametor& eyeParametor);

					IsInEyeTarget_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime
					);

					IsInEyeTarget_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime, 
						const float maxLostIntervalTime,
						const float farRange
					);
				};

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータ
				//--------------------------------------------------------------------------------------
				class IsInEyeTarget : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = IsInEyeTarget_Parametor;

				private:
					const Parametor* m_paramPtr;

					std::weak_ptr<EyeSearchRange> m_eyeRange;
					std::weak_ptr<TargetManager> m_targetManager;

					std::unique_ptr<GameTimer> m_timer;

				public:
					IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~IsInEyeTarget() = default;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					float GetRandomIntervalTime() const;

					/// <summary>
					/// ターゲットが遠すぎるときtrue
					/// </summary>
					/// <returns></returns>
					bool IsFarRange() const;

					/// <summary>
					/// ターゲットを見失っているかどうか
					/// </summary>
					/// <returns></returns>
					bool IsLost() const;

				};

			}
		}
	}
}