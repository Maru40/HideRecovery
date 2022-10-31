/*!
@file I_Decorator.h
@brief I_Decorator�Ȃ�
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

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �Ď��ΏۂɎw�肵���^�[�Q�b�g�����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^
				//--------------------------------------------------------------------------------------
				class IsInEyeTarget : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
					using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

				private:
					//std::weak_ptr<GameObject> m_target;		//�^�[�Q�b�g��ݒ�
					ObserveTargets m_observeTargets;			//�Ď��Ώ۔z��

					std::weak_ptr<EyeSearchRange> m_eyeRange;	//���E�Ǘ��R���|�[�l���g

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
					void AddObserveTarget(const std::shared_ptr<GameObject>& target){ 
						m_observeTargets.push_back(target);
					}

					void SetObserveTargets(const ObserveTargets& targets) { m_observeTargets = targets; }

					void SetObserveTargets(const ObserveSharedTargets& targets);

					_NODISCARD ObserveTargets GetObserveTargets() const noexcept { return m_observeTargets; }

				};

			}
		}
	}
}