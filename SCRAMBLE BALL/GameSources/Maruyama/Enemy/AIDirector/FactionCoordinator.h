
/*!
@file FactionCoordinator.h
@brief FactionCoordinatorなど
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
		class AIDirector;
		class EnemyBase;
		class CombatCoordinator;
		class PatrolCoordinator;
		class FactionCoordinator;
		class I_FactionMember;

		//--------------------------------------------------------------------------------------
		/// グループタイプ
		//--------------------------------------------------------------------------------------
		//enum class FactionType
		//{
		//	None,	//所属していない
		//	Patrol,	//パトロール
		//	Combat,	//戦闘
		//};

		//--------------------------------------------------------------------------------------
		/// グループ調整者
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:
			using CoordinatorsMap = std::unordered_map<type_index, std::vector<std::shared_ptr<CoordinatorBase>>>;

		private:
			std::weak_ptr<AIDirector> m_director;	//AIディレクター

			CoordinatorsMap m_coordinatorsMap;		//それぞれのグループごとのマップ

			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//全てのグループ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="director">AIディレクター</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="director">AIディレクター</param>
			/// <param name="members">メンバー配列</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members);

			virtual ~FactionCoordinator() = default;

			void OnCreate() override;
			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// グループの生成
			/// </summary>
			template<class T, class... Ts,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, T> &&
					std::is_constructible_v<T, const std::shared_ptr<FactionCoordinator>&, Ts...>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<T> CreateFaction(Ts&&... params) {
				auto typeIndex = type_index(typeid(T));

				auto faction = std::make_shared<T>(GetThis<FactionCoordinator>(), params...);
				faction->OnCreate();
				faction->OnStart();
				m_coordinatorsMap[typeIndex].push_back(faction);	//マップに登録
				m_allCoordinators.push_back(faction);

				return faction;
			}

			/// <summary>
			/// メンバーの振り分け場所を探して、返す。
			/// </summary>
			template<class FactionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, FactionType>,	//基底クラスの制限
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<FactionType> SearchAssignFaction() {
				auto typeIndex = type_index(typeid(FactionType));

				//ファクションが存在しないなら、生成
				if (m_coordinatorsMap[typeIndex].size() == 0) {
					return CreateFaction<FactionType>();
				}

				auto coordinators = m_coordinatorsMap[typeIndex];

				//本来なら一番いい条件のファクションを探す。
				auto result = std::dynamic_pointer_cast<FactionType>(coordinators[0]);
				return result;	//(現在は複数のファクションを作る必要がないため、将来的な拡張ポイント)
			}

			/// <summary>
			/// メンバーを指定したファクションにアサインする。
			/// </summary>
			/// <returns>アサインされたファクション</returns>
			template<class FactionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, FactionType>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<FactionType> AddFactionMember(const std::shared_ptr<I_FactionMember>& member) {
				auto typeIndex = type_index(typeid(FactionType));

				//メンバーの振り分けをする。
				auto assignFaction = SearchAssignFaction<FactionType>();
				assignFaction->AddMember(std::dynamic_pointer_cast<EnemyBase>(member));

				return assignFaction;
			}

		public:
			/// <summary>
			/// メンバーのファクション遷移
			/// </summary>
			template<class TransitionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, TransitionType>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<TransitionType> TransitionFaction(const std::shared_ptr<I_FactionMember>& member) {
				//元々アサインされていたメンバーから外す。
				auto assignedFaction = member->GetAssignedFaction();	
				assignedFaction->RemoveMember(std::dynamic_pointer_cast<EnemyBase>(member));

				return AddFactionMember<TransitionType>(member);
			}

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ディレクターの取得
			/// </summary>
			/// <returns>ディレクター</returns>
			std::shared_ptr<AIDirector> GetDirector() const noexcept { return m_director.lock(); }

			//--------------------------------------------------------------------------------------
			/// デバッグ
			//--------------------------------------------------------------------------------------

			//ターゲットを見つけたと知らせるデバッグ処理
			void DebugWriteTarget(const std::shared_ptr<I_FactionMember>& member, const std::shared_ptr<GameObject>& target);
		};

	}
}