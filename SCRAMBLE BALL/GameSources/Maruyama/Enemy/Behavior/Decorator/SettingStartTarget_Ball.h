/*!
@file SettingStartTarget_Ball.h
@brief SettingStartTarget_Ball�Ȃ�
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
				/// �J�n���Ƀ{�[���^�[�Q�b�g���Z�b�e�B���O������B
				//--------------------------------------------------------------------------------------
				class SettingStartTarget_Ball : public DecoratorBase<GameObject>
				{
					std::weak_ptr<GameObject> m_target;
					std::weak_ptr<TargetManager> m_targetManager;

				public:
					SettingStartTarget_Ball(
						const std::shared_ptr<GameObject>& owner,
						const std::shared_ptr<GameObject>& target
					);

					void OnStart() override;

					void OnExit() override;

					bool CanTransition() const override { return true; }

					bool CanUpdate() override { return true; }
				};

			}
		}
	}
}