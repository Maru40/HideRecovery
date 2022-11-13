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
				/// 開始時に特定のターゲットをセットする。(現在使用不可)(将来的に条件式など汎用性の高いデコレータにするためのクラス)
				//--------------------------------------------------------------------------------------
				class SettingStartTarget : public DecoratorBase<GameObject>
				{
					std::vector<std::weak_ptr<GameObject>> m_targets;	//ターゲット候補

				public:
					SettingStartTarget(
						const std::shared_ptr<GameObject>& owner,
						const std::vector<std::weak_ptr<GameObject>> targets
					) :
						DecoratorBase(owner),
						m_targets(targets)
					{}

					void OnStart() override;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }
				};

			}
		}
	}
}