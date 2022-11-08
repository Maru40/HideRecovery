
/*!
@file CombatCoordinator.h
@brief CombatCoordinatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// 戦闘データのタイプ
		//--------------------------------------------------------------------------------------
		enum class CombatCoordinator_Data_State
		{
			Move,
			Attack,
			Evade,
			Wait,
		};

		//--------------------------------------------------------------------------------------
		/// 戦闘管理データ
		//--------------------------------------------------------------------------------------
		struct CombatCoordinator_Data
		{
			using State = CombatCoordinator_Data_State;

			std::weak_ptr<EnemyBase> selfPtr;
			State state = State::Move;

			//CombatCoordinator_Data();
		};

		//--------------------------------------------------------------------------------------
		/// 戦闘管理、攻撃権限管理、ロールアサイン
		//--------------------------------------------------------------------------------------
		class CombatCoordinator : public HereOwnerCoordinatorBase<FactionCoordinator, EnemyBase>
		{
		public:
			
		private:
			std::vector<std::weak_ptr<GameObject>> m_targets;	//すでに発見済みのターゲット

		public:
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~CombatCoordinator() = default;

			void OnCreate() override;
			void OnStart() override {};
			bool OnUpdate() override;
			void OnExit() override {};
			
		private:

		private:
			/// <summary>
			/// ターゲットの監視
			/// </summary>
			void UpdateObserveFindTarget();

			/// <summary>
			/// メンバーがダメージを受けたことを監視
			/// </summary>
			void UpdateObserveDamaged();

			/// <summary>
			/// 同じリクエスタのタプルを削除(本来ここに書くべきじゃない。移動予定)
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
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			void AddTargets(const std::shared_ptr<GameObject>& target);

			bool HasTarget(const std::shared_ptr<GameObject>& target) const;
		};

	}
}