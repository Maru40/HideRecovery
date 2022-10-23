
/*!
@file HidePlacePatrol.h
@brief HidePlacePatrol�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "../PatrolCoordinator.h"

namespace basecross {

	class HidePlace;

	namespace Enemy {

		namespace AICoordinator {

			namespace Patrol {

				//--------------------------------------------------------------------------------------
				/// �B���ꏊ�̃p�g���[�������҂̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct HidePlacePatrol_Parametor {

				};

				//--------------------------------------------------------------------------------------
				/// �B���ꏊ�̃p�g���[��������
				//--------------------------------------------------------------------------------------
				class HidePlacePatrol : public PatrolCoordinator
				{
				public:
					using Parametor = HidePlacePatrol_Parametor;

				private:
					Parametor m_param;

				public:
					HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner);

					HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members);

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override {}

				private:


				public:
					/// <summary>
					/// �����o�[����p�g���[���^�[�Q�b�g�̌������˗����ꂽ���̏���
					/// </summary>
					/// <param name="member">�����˗����o���������o�[</param>
					/// <returns>�p�g���[���^�[�Q�b�g</returns>
					std::shared_ptr<GameObject> SearchTarget(const std::shared_ptr<I_FactionMember>& member);

					//--------------------------------------------------------------------------------------
					/// �A�N�Z�b�T
					//--------------------------------------------------------------------------------------

					bool IsEnd() const;

				};

			}

		}
	}
}