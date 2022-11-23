/*!
@file NoneHidePlace.h
@brief NoneHidePlace�Ȃ�
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

	class GameTimer;
	class TargetManager;
	class EyeSearchRange;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�����E�O�Ɉ�莞�Ԃ���ƁA�����I��������f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct OutTargetTimer_Parametor {
					EyeSearchRangeParametor eyeParametor;	//���E�p�����[�^
					float minLostIntervalTime;				//����������ɒǂ������鎞��(�ŏ�)
					float maxLostIntervalTime;				//����������ɒǂ������鎞��(�ő�)
					float farRange;							//�Ǐ]�s�\�Ȓ������ɍs�����Ɣ��f���鋗��

					OutTargetTimer_Parametor();

					OutTargetTimer_Parametor(const EyeSearchRangeParametor& eyeParametor);

					OutTargetTimer_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime
					);

					OutTargetTimer_Parametor(
						const EyeSearchRangeParametor& eyeParametor,
						const float minLostIntervalTime,
						const float maxLostIntervalTime,
						const float farRange
					);
				};

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�����E�O�Ɉ�莞�Ԃ���ƁA�����I��������f�R���[�^
				//--------------------------------------------------------------------------------------
				class OutTargetTimer : public DecoratorBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = OutTargetTimer_Parametor;

				private:
				    const Parametor* m_paramPtr;							//�p�����[�^

					std::unique_ptr<GameTimer> m_timer;				//�^�C�}�[

					std::weak_ptr<TargetManager> m_targetManager;	//�^�[�Q�b�g�Ǘ�
					std::weak_ptr<EyeSearchRange> m_eyeRange;		//���E�Ǘ�

				public:
					OutTargetTimer(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

					virtual ~OutTargetTimer() = default;

					void OnStart() override;

					bool CanTransition() const override { return true; }

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

					//�^�[�Q�b�g�����S�Ƀ��X�g
					void LostTarget();

				};

			}
		}
	}
}