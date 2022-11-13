/*!
@file SettingStartTarget_Goal.h
@brief SettingStartTarget_Goal‚È‚Ç
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

				class SettingStartTarget_Goal : public DecoratorBase<GameObject>
				{
					std::weak_ptr<I_TeamMember> m_teamMember;
					std::weak_ptr<TargetManager> m_targetManager;

				public:
					SettingStartTarget_Goal(const std::shared_ptr<GameObject>& owner);

					void OnStart() override;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }
				};

			}
		}
	}
}