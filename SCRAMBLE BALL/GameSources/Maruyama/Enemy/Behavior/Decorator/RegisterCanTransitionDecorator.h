/*!
@file OutSpecificTarget.h
@brief OutSpecificTarget�Ȃ�
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
				/// �J�ڏ�����o�^����f�R���[�^
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