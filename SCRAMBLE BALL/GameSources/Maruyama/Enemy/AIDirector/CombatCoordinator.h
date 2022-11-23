
/*!
@file CombatCoordinator.h
@brief CombatCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		namespace Tuple {
			class Kill;
			class SearchTarget;
			class FindTarget;
			class LostTarget;
		}

		//--------------------------------------------------------------------------------------
		/// �퓬�f�[�^�̃^�C�v
		//--------------------------------------------------------------------------------------
		enum class CombatCoordinator_Data_State
		{
			Move,
			Attack,
			Evade,
			Wait,
		};

		//--------------------------------------------------------------------------------------
		/// �퓬�Ǘ��f�[�^
		//--------------------------------------------------------------------------------------
		struct CombatCoordinator_Data
		{
			using State = CombatCoordinator_Data_State;

			std::weak_ptr<EnemyBase> selfPtr;
			State state = State::Move;

			//CombatCoordinator_Data();
		};

		//--------------------------------------------------------------------------------------
		/// �퓬�Ǘ��A�U�������Ǘ��A���[���A�T�C��
		//--------------------------------------------------------------------------------------
		class CombatCoordinator : public HereOwnerCoordinatorBase<FactionCoordinator, EnemyBase>
		{
		public:
			
		private:
			std::vector<std::weak_ptr<GameObject>> m_targets;	//���łɔ����ς݂̃^�[�Q�b�g

		public:
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~CombatCoordinator() = default;

			void OnCreate() override;
			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
			
		private:

		private:
			/// <summary>
			/// �^�[�Q�b�g�̊Ď�
			/// </summary>
			void UpdateObserveFindTarget();

			/// <summary>
			/// �����o�[���_���[�W���󂯂����Ƃ��Ď�
			/// </summary>
			void UpdateObserveDamaged();

			/// <summary>
			/// �L���^�v�����󂯎�����Ƃ��B
			/// </summary>
			void NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple);

			/// <summary>
			/// �^�[�Q�b�g�̌���������ꂽ���B
			/// </summary>
			/// <param name="tuple"></param>
			void NotifyTuple_SearchTarget(const std::shared_ptr<Tuple::SearchTarget>& tuple);

			/// <summary>
			/// ��ԗD��x�̍����^�[�Q�b�g��T���ĕԂ��B
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<GameObject> SearchPriorityTarget(const std::shared_ptr<GameObject>& requester);

			void NotifyTuple_FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

			void NotifyTuple_LostTarget(const std::shared_ptr<Tuple::LostTarget>& tuple);

			/// <summary>
			/// �������N�G�X�^�̃^�v�����폜(�{�������ɏ����ׂ�����Ȃ��B�ړ��\��)
			/// </summary>
			template<class T>
			void RemoveTuples(std::vector<std::shared_ptr<T>>& tuples, std::shared_ptr<Enemy::Tuple::I_Tupler>& requester) {
				auto iter = tuples.begin();
				while (iter != tuples.end()) {
					if ((*iter)->GetRequester() == requester) {
						iter = tuples.erase(iter);
						continue;
					}
					iter++;
				}
			}

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			void AddTarget(const std::shared_ptr<GameObject>& target);

			bool RemoveTaret(const std::shared_ptr<GameObject>& target);

			bool HasTarget(const std::shared_ptr<GameObject>& target) const;

			/// <summary>
			/// �����ς݂̃^�[�Q�b�g���擾����B
			/// </summary>
			/// <returns>�����ς݂̃^�[�Q�b�g</returns>
			std::vector<std::weak_ptr<GameObject>> GetFindTargets() const { return m_targets; }
		};

	}
}