/*!
@file IsInEyeTargets.h
@brief IsInEyeTargets�Ȃ�
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
	class TargetManager;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �����̊Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct IsInEyeTargets_Parametor {
					float lostIntervalTime;	//����������ɒǂ������鎞��
					float farRange;			//�Ǐ]�s�\�Ȓ������ɍs�����Ɣ��f���鋗��

					IsInEyeTargets_Parametor();

					IsInEyeTargets_Parametor(const float lostIntervalTime, const float farRange);
				};

				//--------------------------------------------------------------------------------------
				/// �����̊Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^
				//--------------------------------------------------------------------------------------
				class IsInEyeTargets : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
					using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

					using Parametor = IsInEyeTargets_Parametor;

				private:
					Parametor m_param;	//�p�����[�^

					std::unique_ptr<GameTimer> m_timer;							//���ԊǗ�
					std::unique_ptr<ObserveIsInEyeTarget> m_observeIsInTarget;	//�Ď������S���N���X�B

					std::weak_ptr<EyeSearchRange> m_eyeRange;					//���E�Ǘ��N���X
					std::weak_ptr<TargetManager> m_targetManager;				//�ڕW�Ď��N���X
					
				public:
					IsInEyeTargets(const std::shared_ptr<Enemy::EnemyBase>& owner);

					IsInEyeTargets(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveSharedTargets& observeTargets,
						const Parametor& parametor = Parametor()
					);

					IsInEyeTargets(
						const std::shared_ptr<Enemy::EnemyBase>& owner,
						const ObserveTargets& observeTargets,
						const Parametor& parametor = Parametor()
					);

					virtual ~IsInEyeTargets() = default;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					bool IsLost() const;

					/// <summary>
					/// �ڕW�������ɂ��邩�ǂ���
					/// </summary>
					/// <returns>�����ɂ���Ȃ�true</returns>
					bool IsFarRange() const;

				public:

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

					void SetLostIntervalTime(const float time) noexcept { m_param.lostIntervalTime = time; };

					_NODISCARD float GetLostIntervalTime() const noexcept { return m_param.lostIntervalTime; }
				};

			}
		}
	}
}