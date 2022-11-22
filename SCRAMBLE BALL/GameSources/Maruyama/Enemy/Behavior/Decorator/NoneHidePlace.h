/*!
@file NoneHidePlace.h
@brief NoneHidePlace‚È‚Ç
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
				/// ‰B‚µêŠ‚ª‘S‚Ä‹ó‚¢‚Ä‚¢‚ÄA’T‚·êŠ‚ª‚È‚¢‚È‚ç
				//--------------------------------------------------------------------------------------
				class NoneHidePlace : public DecoratorBase<Enemy::EnemyBase>
				{
				public:

				};

			}
		}
	}
}