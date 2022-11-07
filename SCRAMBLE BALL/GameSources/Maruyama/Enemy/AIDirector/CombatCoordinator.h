
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
			using Data = CombatCoordinator_Data;

		private:
			std::map<std::weak_ptr<EnemyBase>, Data> m_dataMap;	//エネミーごとのデータマップ
			std::vector<Data> m_requestDatas;					//リクエストデータ管理配列

		public:
			CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~CombatCoordinator() = default;

			void OnStart() override {};
			bool OnUpdate() override;
			void OnExit() override {};
			
		private:
			//命令の生成
			void CreateOrder(const Data& data);

		private:
			/// <summary>
			/// ターゲットの監視
			/// </summary>
			void ObserveFindTarget();

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

			/// <summary>
			/// 行動のリクエスト
			/// </summary>
			/// <param name="member">リクエストを送ったメンバー</param>
			/// <param name="data">リクエストデータ</param>
			void Request(const std::shared_ptr<EnemyBase>& member, const Data& data);

			
		};

	}
}