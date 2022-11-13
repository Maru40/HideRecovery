/*!
@file IsSpecificTarget.h
@brief IsSpecificTarget�Ȃ�
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
				/// ����̃^�[�Q�b�g�̎�������̏�ԂȂ�J�ڂł���(���ݎg�p�s��)
				//--------------------------------------------------------------------------------------
				class IsSpecificTarget : public DecoratorBase<GameObject>
				{
					std::weak_ptr<GameObject> m_target;	//�^�[�Q�b�g

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