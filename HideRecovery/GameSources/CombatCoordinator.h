
/*!
@file CombatCoordinator.h
@brief CombatCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		class EnemyBase;
		class FactionCoordinator;

		enum class CombatCoordinator_Data_State
		{
			Move,
			Attack,
			Evade,
			Wait,
		};

		struct CombatCoordinator_Data
		{
			using State = CombatCoordinator_Data_State;

			//�����o�ϐ�-------------------------------------------

			ex_weak_ptr<EnemyBase> selfPtr = nullptr;
			State state = State::Move;

			//CombatCoordinator_Data();
		};


		//�퓬�Ǘ��A�U�������Ǘ��A���[���A�T�C��
		class CombatCoordinator
		{
		public:
			using Data = CombatCoordinator_Data;

		private:
			ex_weak_ptr<FactionCoordinator> m_owner;
			vector<ex_weak_ptr<EnemyBase>> m_members;

			std::map<ex_weak_ptr<EnemyBase>, Data> m_dataMap;
			std::vector<Data> m_requestDatas;

		public:
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner);
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const std::shared_ptr<EnemyBase>& member);
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const vector<ex_weak_ptr<EnemyBase>>& members);

			//void OnStart();
			void OnUpdate();
			//void OnExit();
			
		private:
			//���߂̐���
			void CreateOrder(const Data& data);

		public:

			//�A�N�Z�b�T----------------------------------------------------------------

			//�����o�[�̃A�T�C��
			void Assign(const std::shared_ptr<EnemyBase>& member);
			//�����o�[�̃A���A�T�C��
			void UnAssign(const std::shared_ptr<EnemyBase>& member);

			//�s���̃��N�G�X�g
			void Request(const std::shared_ptr<EnemyBase>& member, const Data& data);

			std::shared_ptr<FactionCoordinator> GetOwner() const;
		};

	}
}