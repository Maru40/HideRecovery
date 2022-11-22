/*!
@file NoneHidePlace.h
@brief NoneHidePlace�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------

	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �B���ꏊ���S�ċ󂢂Ă��āA�T���ꏊ���Ȃ��Ȃ�
				//--------------------------------------------------------------------------------------
				class NoneHidePlace : public DecoratorBase<Enemy::EnemyBase>
				{
				public:

				};

			}
		}
	}
}