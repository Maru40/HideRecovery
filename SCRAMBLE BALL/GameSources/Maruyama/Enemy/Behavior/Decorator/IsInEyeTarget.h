/*!
@file IsInEyeTarget.h
@brief IsInEyeTarget�Ȃ�
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
	class EyeSearchRange;
	class ObserveIsInEyeTarget;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^
				//--------------------------------------------------------------------------------------
				class IsInEyeTarget : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
					using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

				private:

					std::unique_ptr<ObserveIsInEyeTarget> m_observeIsInTarget;	//�Ď������S���N���X�B
					
				public:
					IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner);

					IsInEyeTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveSharedTargets& observeTargets
					);

					IsInEyeTarget(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveTargets& observeTargets
					);

					virtual ~IsInEyeTarget() = default;

					bool CanTransition() const override;

					bool CanUpdate() override;

					//--------------------------------------------------------------------------------------
					/// �A�N�Z�b�T
					//--------------------------------------------------------------------------------------

					/// <summary>
					/// �Ď��Ώۂ̒ǉ�
					/// </summary>
					/// <param name="target">�Ď��Ώ�</param>
					void AddObserveTarget(const std::shared_ptr<GameObject>& target);

					void SetObserveTargets(const ObserveTargets& targets);

					void SetObserveTargets(const ObserveSharedTargets& targets);

					_NODISCARD ObserveTargets GetObserveTargets() const noexcept;

				};

			}
		}
	}
}