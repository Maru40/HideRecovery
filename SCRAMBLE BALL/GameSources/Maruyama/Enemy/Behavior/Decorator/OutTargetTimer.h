/*!
@file NoneHidePlace.h
@brief NoneHidePlaceなど
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

	class GameTimer;
	class TargetManager;
	class EyeSearchRange;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ターゲットが視界外に一定時間いると、強制終了させるデコレータのパラメータ
				//--------------------------------------------------------------------------------------
				struct OutTargetTimer_Parametor {
					EyeSearchRangeParametor eyeParametor;	//視界パラメータ
					float minLostIntervalTime;				//見失った後に追いかける時間(最小)
					float maxLostIntervalTime;				//見失った後に追いかける時間(最大)
					float farRange;							//追従不可能な程遠くに行ったと判断する距離

					OutTargetTimer_Parametor();

					OutTargetTimer_Parametor(const EyeSearchRangeParametor& eyeParametor);

					OutTargetTimer_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime
					);

					OutTargetTimer_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime,
						const float farRange
					);
				};

				//--------------------------------------------------------------------------------------
				/// ターゲットが視界外に一定時間いると、強制終了させるデコレータ
				//--------------------------------------------------------------------------------------
				class OutTargetTimer : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = OutTargetTimer_Parametor;

				private:
				    const Parametor* m_paramPtr;							//パラメータ

					std::unique_ptr<GameTimer> m_timer;				//タイマー

					std::weak_ptr<TargetManager> m_targetManager;	//ターゲット管理
					std::weak_ptr<EyeSearchRange> m_eyeRange;		//視界管理

				public:
					OutTargetTimer(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~OutTargetTimer() = default;

					void OnStart() override;

					bool CanTransition() const override { return true; }

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