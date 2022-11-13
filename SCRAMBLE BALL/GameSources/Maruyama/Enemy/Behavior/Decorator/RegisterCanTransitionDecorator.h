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
				/// �J�ڏ�����o�^����f�R���[�^
				//--------------------------------------------------------------------------------------
				class ResgisterCanTrasitionDecorator : public I_Decorator
				{
					std::vector<std::function<void()>> m_startFunctions;		//�J�n���ɌĂяo������
					std::vector<std::function<void()>> m_reserveCanTransitions;	//�J�ڏ����m�F�O�ɌĂяo������
					std::vector<std::function<bool()>> m_canTransitions;		//�J�ڎ��ɌĂяo������
					std::vector<std::function<bool()>> m_canUpdates;			//�X�V���ɌĂяo������

				public:
					void OnCreate() override {}

					void OnStart() override;

					void ReserveCanTransition() override;

					bool CanTransition() const override;

					bool CanUpdate() override;

				public:
					void AddStartFunction(const std::function<void()>& startFunction);

					void AddReserveCanTransitionFunction(const std::function<void()>& function);

					void AddCanTransitionFunction(const std::function<bool()>& canTransition);

					void AddCanUpdateFunction(const std::function<bool()>& canUpdate);
				};

			}
		}
	}
}