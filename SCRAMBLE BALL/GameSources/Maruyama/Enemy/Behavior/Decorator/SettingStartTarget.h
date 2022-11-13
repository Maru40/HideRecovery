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
				/// �J�n���ɓ���̃^�[�Q�b�g���Z�b�g����B(���ݎg�p�s��)(�����I�ɏ������Ȃǔėp���̍����f�R���[�^�ɂ��邽�߂̃N���X)
				//--------------------------------------------------------------------------------------
				class SettingStartTarget : public DecoratorBase<GameObject>
				{
					std::vector<std::weak_ptr<GameObject>> m_targets;	//�^�[�Q�b�g���

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