
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
			using Data = CombatCoordinator_Data;

		private:
			std::map<std::weak_ptr<EnemyBase>, Data> m_dataMap;	//�G�l�~�[���Ƃ̃f�[�^�}�b�v
			std::vector<Data> m_requestDatas;					//���N�G�X�g�f�[�^�Ǘ��z��

		public:
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~CombatCoordinator() = default;

			void OnStart() override {};
			bool OnUpdate() override;
			void OnExit() override {};
			
		private:
			//���߂̐���
			void CreateOrder(const Data& data);

		private:
			/// <summary>
			/// �^�[�Q�b�g�̊Ď�
			/// </summary>
			void ObserveFindTarget();

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

			/// <summary>
			/// �s���̃��N�G�X�g
			/// </summary>
			/// <param name="member">���N�G�X�g�𑗂��������o�[</param>
			/// <param name="data">���N�G�X�g�f�[�^</param>
			void Request(const std::shared_ptr<EnemyBase>& member, const Data& data);

			
		};

	}
}