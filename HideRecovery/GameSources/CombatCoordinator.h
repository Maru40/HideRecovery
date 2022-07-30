
/*!
@file CombatCoordinator.h
@brief CombatCoordinatorなど
担当：丸山裕喜
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

			//メンバ変数-------------------------------------------

			ex_weak_ptr<EnemyBase> selfPtr = nullptr;
			State state = State::Move;

			//CombatCoordinator_Data();
		};


		//戦闘管理、攻撃権限管理、ロールアサイン
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
			//命令の生成
			void CreateOrder(const Data& data);

		public:

			//アクセッサ----------------------------------------------------------------

			//メンバーのアサイン
			void Assign(const std::shared_ptr<EnemyBase>& member);
			//メンバーのアンアサイン
			void UnAssign(const std::shared_ptr<EnemyBase>& member);

			//行動のリクエスト
			void Request(const std::shared_ptr<EnemyBase>& member, const Data& data);

			std::shared_ptr<FactionCoordinator> GetOwner() const;
		};

	}
}