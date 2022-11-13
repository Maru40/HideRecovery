/*!
@file OutSpecificTarget.h
@brief OutSpecificTarget‚È‚Ç
’S“–ÒFŠÛR —TŠì
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ‘O•ûéŒ¾
	//--------------------------------------------------------------------------------------

	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ‘JˆÚğŒ‚ğ“o˜^‚·‚éƒfƒRƒŒ[ƒ^
				//--------------------------------------------------------------------------------------
				class ResgisterCanTrasitionDecorator : public I_Decorator
				{
					std::vector<std::function<bool()>> m_canTransitions;

				public:
					void OnStart() override {}

					bool CanTransition() const override;

					bool CanUpdate() override;

				public:
					void AddCanTransitionFunction(const std::function<bool()>& canTransition);

				};

			}
		}
	}
}