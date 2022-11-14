/*!
@file TimerWaitPosition.h
@brief TimerWaitPositionなど
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

	class GameTimer;
	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 一定時間同じ場所にとどまっていたら一度タスクをリセットするために、戻るデコレータのパラメータ
				//--------------------------------------------------------------------------------------
				struct TimerWaitPosition_Parametor {
					float time;			//待機に使う時間
					float updateRange;	//更新する距離(この距離以上動いたら、止まってはいない。)

					TimerWaitPosition_Parametor();

					~TimerWaitPosition_Parametor() = default;
				};

				//--------------------------------------------------------------------------------------
				/// 一定時間同じ場所にとどまっていたら一度タスクをリセットするために、戻るデコレータ
				//--------------------------------------------------------------------------------------
				class TimerWaitPosition : public DecoratorBase<GameObject>
				{
				public:
					using Parametor = TimerWaitPosition_Parametor;

				private:
					Parametor m_param;	//パラメータ

					Vec3 m_observePosition;	//監視する場所

					std::weak_ptr<Transform> m_transform;

					std::unique_ptr<GameTimer> m_timer;	//タイマー

				public:
					TimerWaitPosition(const std::shared_ptr<GameObject>& owner);

					virtual ~TimerWaitPosition() = default;

					void OnStart() override;

					bool CanTransition() const override { return true; };

					bool CanUpdate() override;

				private:
					/// <summary>
					/// 監視登録されている場所を更新
					/// </summary>
					void UpdatePosition();

					/// <summary>
					/// 監視登録されている場所を更新するかどうか
					/// </summary>
					/// <returns></returns>
					bool IsUpdatePosition();
				};

			}
		}
	}
}