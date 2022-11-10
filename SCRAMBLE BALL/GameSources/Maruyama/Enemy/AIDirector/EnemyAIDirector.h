
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirectorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

namespace basecross {

	namespace team {
		enum class TeamType;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		namespace Tuple {
			class FindBall;
		}

		//--------------------------------------------------------------------------------------
		/// フィールド全体の制御
		//--------------------------------------------------------------------------------------
		class AIDirector : public maru::SingletonComponent<AIDirector>, public Tuple::I_Tupler
		{
			//全てのエネミー
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//グループ管理の配列
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

			//タプルスペース
			std::shared_ptr<Tuple::TupleSpace> m_tupleSapce;

		public :
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnLateStart() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// ボールを見つけた通知を受け取ったときの処理。
			/// </summary>
			void NotifyTuple_FindBall(const std::shared_ptr<Tuple::FindBall>& tuple);

			/// <summary>
			/// 初期配置されたエネミーを全て取得
			/// </summary>
			void SettingStartAllEnemys();

			/// <summary>
			/// グループ管理の生成
			/// </summary>
			/// <returns>生成した派閥</returns>
			std::shared_ptr<FactionCoordinator> CreateFaction(const std::vector<std::weak_ptr<EnemyBase>>& assignMembers);

			/// <summary>
			/// グループ管理の削除
			/// </summary>
			/// <param name="removeCoordinator">削除したい派閥</param>
			/// <returns>削除できたらtrue</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

			/// <summary>
			/// チームごとにマップに敵を分けて返す。
			/// </summary>
			/// <param name="enemys">チームごとに分けたいメンバー</param>
			/// <returns>マップでチームごとに分けたエネミー配列</returns>
			std::unordered_map<team::TeamType, std::vector<weak_ptr<EnemyBase>>> DivideTeamType(const std::vector<std::weak_ptr<EnemyBase>>& members);

		public:
			/// <summary>
			/// ゲーム開始時にAIDirectorに全ての敵をアサインする処理
			/// </summary>
			void StartAssign();

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// エネミーの追加
			/// </summary>
			/// <param name="enemy">エネミー</param>
			void AddEnemy(const std::shared_ptr<EnemyBase>& enemy);

			/// <summary>
			/// グループ管理の取得
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator(const int index) const;

			/// <summary>
			/// タプルスペースの取得
			/// </summary>
			/// <returns></returns>
			_NODISCARD std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept override;

		};

	}
}