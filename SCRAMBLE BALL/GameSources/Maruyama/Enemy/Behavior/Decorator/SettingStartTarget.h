/*!
@file SettingStartTarget.h
@brief SettingStartTargetなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

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
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<Enemy::Tuple::I_Tuple, T>,	//タプルスペースを呼ぶ。
					std::nullptr_t> = nullptr
				>
				class SettingStartTarget : public DecoratorBase<GameObject>
				{
				public:
					SettingStartTarget(
						const std::shared_ptr<GameObject>& owner
					) :
						DecoratorBase(owner)
					{}

					void OnStart() override {};

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }
				};

			}
		}
	}
}