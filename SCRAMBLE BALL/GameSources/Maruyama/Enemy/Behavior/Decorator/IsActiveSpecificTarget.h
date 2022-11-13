/*!
@file IsActiveSpecificTarget.h
@brief IsActiveSpecificTarget�Ȃ�
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
				/// ����̃^�[�Q�b�g�̎���Active��Ԃ̎��̂ݑJ�ڂ���B
				//--------------------------------------------------------------------------------------
				class IsAcitiveSpecificTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<GameObject> m_target;	//�^�[�Q�b�g

				public:
					IsAcitiveSpecificTarget(
						const std::shared_ptr<GameObject>& owner,
						const std::shared_ptr<GameObject>& target
					);

					bool CanTransition() const override;

					bool CanUpdate() override;

				};

			}
		}
	}
}