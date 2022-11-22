
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

		class I_FactionMember;

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

					virtual ~HidePlacePatrol() = default;

					void OnCreate() override;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:
					
					void ObserveTransitionButtle();

					//�^�[�Q�b�g�������������ɌĂяo������
					void FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

					/// <summary>
					/// ���łɃ����o�[���^�[�Q�b�g�w�肵�Ă��邩�ǂ���
					/// </summary>
					/// <param name="target">�m�F�������^�[�Q�b�g</param>
					/// <returns>���łɕʂ̃����o�[���^�[�Q�b�g�Ɏw�肵�Ă�����true</returns>
					bool IsSomeMemberTarget(const std::shared_ptr<GameObject>& target);

					/// <summary>
					/// ���̃����o�\���^�[�Q�b�g�ɂ��Ă��Ȃ��A�^�[�Q�b�g�����擾
					/// </summary>
					/// <param name="member">�����˗����o���������o�[</param>
					/// <returns>���̃����o�\���^�[�Q�b�g�ɂ��Ă��Ȃ��A�^�[�Q�b�g���̔z��</returns>
					std::vector<std::shared_ptr<GameObject>> SearchOtherTargets(
						const std::shared_ptr<I_FactionMember>& member,
						const std::vector<std::weak_ptr<HidePlace>>& hidePlaces
					);

					/// <summary>
					/// ���̃����o�[���^�[�Q�b�g�ɂ��Ă���B���ꏊ���擾����(���ɒT���ꏊ���Ȃ�����A�����ꏊ��T���B)
					/// </summary>
					/// <returns></returns>
					std::shared_ptr<GameObject> SearchSomeTarget() const;

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