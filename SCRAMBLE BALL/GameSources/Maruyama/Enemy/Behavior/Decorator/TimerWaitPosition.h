/*!
@file TimerWaitPosition.h
@brief TimerWaitPosition�Ȃ�
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

	class GameTimer;
	class TargetManager;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ��莞�ԓ����ꏊ�ɂƂǂ܂��Ă������x�^�X�N�����Z�b�g���邽�߂ɁA�߂�f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct TimerWaitPosition_Parametor {
					float time;			//�ҋ@�Ɏg������
					float updateRange;	//�X�V���鋗��(���̋����ȏ㓮������A�~�܂��Ă͂��Ȃ��B)

					TimerWaitPosition_Parametor();

					~TimerWaitPosition_Parametor() = default;
				};

				//--------------------------------------------------------------------------------------
				/// ��莞�ԓ����ꏊ�ɂƂǂ܂��Ă������x�^�X�N�����Z�b�g���邽�߂ɁA�߂�f�R���[�^
				//--------------------------------------------------------------------------------------
				class TimerWaitPosition : public DecoratorBase<GameObject>
				{
				public:
					using Parametor = TimerWaitPosition_Parametor;

				private:
					Parametor m_param;	//�p�����[�^

					Vec3 m_observePosition;	//�Ď�����ꏊ

					std::weak_ptr<Transform> m_transform;

					std::unique_ptr<GameTimer> m_timer;	//�^�C�}�[

				public:
					TimerWaitPosition(const std::shared_ptr<GameObject>& owner);

					virtual ~TimerWaitPosition() = default;

					void OnStart() override;

					bool CanTransition() const override { return true; };

					bool CanUpdate() override;

				private:
					/// <summary>
					/// �Ď��o�^����Ă���ꏊ���X�V
					/// </summary>
					void UpdatePosition();

					/// <summary>
					/// �Ď��o�^����Ă���ꏊ���X�V���邩�ǂ���
					/// </summary>
					/// <returns></returns>
					bool IsUpdatePosition();
				};

			}
		}
	}
}