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
		class I_FactionMember;
		
		namespace Tuple {
			class I_Tupler;
		}
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 現在のターゲットが現在もターゲット指定可能かどうかを判断
				//--------------------------------------------------------------------------------------
				class CanCurrentTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<TargetManager> m_targetManager;
					std::weak_ptr<Enemy::I_FactionMember> m_factionMember;
					std::weak_ptr<Enemy::Tuple::I_Tupler> m_tupler;

				public:
					CanCurrentTarget(const std::shared_ptr<GameObject>& owner);

					void ReserveCanTransition() override;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					void RemoveFactionTarget(const std::shared_ptr<GameObject>& target);

				};

			}
		}
	}
}