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
				/// 遷移条件を登録するデコレータ
				//--------------------------------------------------------------------------------------
				class ResgisterCanTrasitionDecorator : public I_Decorator
				{
					std::vector<std::function<void()>> m_startFunctions;		//開始時に呼び出す処理
					std::vector<std::function<void()>> m_reserveCanTransitions;	//遷移条件確認前に呼び出す処理
					std::vector<std::function<bool()>> m_canTransitions;		//遷移時に呼び出す処理
					std::vector<std::function<bool()>> m_canUpdates;			//更新時に呼び出す処理

				public:
					void OnCreate() override {}

					void OnStart() override;

					void ReserveCanTransition() override;

					bool CanTransition() const override;

					bool CanUpdate() override;

				public:
					void AddStartFunction(const std::function<void()>& startFunction);

					void AddReserveCanTransitionFunction(const std::function<void()>& function);

					void AddCanTransitionFunction(const std::function<bool()>& canTransition);

					void AddCanUpdateFunction(const std::function<bool()>& canUpdate);
				};

			}
		}
	}
}