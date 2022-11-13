/*!
@file IsSpecificTarget.h
@brief IsSpecificTargetなど
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
				/// 特定のターゲットの時が特定の状態なら遷移できる(現在使用不可)
				//--------------------------------------------------------------------------------------
				class IsSpecificTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<GameObject> m_target;	//ターゲット

				public:
					IsSpecificTarget(
						const std::shared_ptr<GameObject>& owner,
						const std::shared_ptr<GameObject>& target
					);
				};

			}
		}
	}
}