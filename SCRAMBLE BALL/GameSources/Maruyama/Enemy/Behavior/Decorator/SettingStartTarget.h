/*!
@file SettingStartTarget.h
@brief SettingStartTarget�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

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
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<Enemy::Tuple::I_Tuple, T>,	//�^�v���X�y�[�X���ĂԁB
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