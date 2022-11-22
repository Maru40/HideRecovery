/*!
@file IsInEyeTarget.h
@brief IsInEyeTarget�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;
	class TargetManager;
	class GameTimer;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				enum class IsInEyeTarget_CanType {
					In,
					Out
				};

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct IsInEyeTarget_Parametor {
					EyeSearchRangeParametor eyeParametor;	//���E�p�����[�^
					float minLostIntervalTime;				//����������ɒǂ������鎞��(�ŏ�)
					float maxLostIntervalTime;				//����������ɒǂ������鎞��(�ő�)
					float farRange;							//�Ǐ]�s�\�Ȓ������ɍs�����Ɣ��f���鋗��

					IsInEyeTarget_Parametor();

					IsInEyeTarget_Parametor(const EyeSearchRangeParametor& eyeParametor);

					IsInEyeTarget_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime
					);

					IsInEyeTarget_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime, 
						const float maxLostIntervalTime,
						const float farRange
					);
				};

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^
				//--------------------------------------------------------------------------------------
				class IsInEyeTarget : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = IsInEyeTarget_Parametor;

				private:
					const Parametor* m_paramPtr;

					std::weak_ptr<EyeSearchRange> m_eyeRange;
					std::weak_ptr<TargetManager> m_targetManager;

					std::unique_ptr<GameTimer> m_timer;

				public:
					IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~IsInEyeTarget() = default;

					void OnStart() override;

					bool CanTransition() const override;

					bool CanUpdate() override;

				private:
					float GetRandomIntervalTime() const;

					/// <summary>
					/// �^�[�Q�b�g����������Ƃ�true
					/// </summary>
					/// <returns></returns>
					bool IsFarRange() const;

					/// <summary>
					/// �^�[�Q�b�g���������Ă��邩�ǂ���
					/// </summary>
					/// <returns></returns>
					bool IsLost() const;

				};

			}
		}
	}
}