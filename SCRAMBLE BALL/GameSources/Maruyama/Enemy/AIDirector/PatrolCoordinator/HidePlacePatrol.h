
/*!
@file HidePlacePatrol.h
@brief HidePlacePatrolなど
担当：丸山裕喜
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
				/// 隠し場所のパトロール調整者のパラメータ
				//--------------------------------------------------------------------------------------
				struct HidePlacePatrol_Parametor {

				};

				//--------------------------------------------------------------------------------------
				/// 隠し場所のパトロール調整者
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
					/// メンバーからパトロールターゲットの検索を依頼された時の処理
					/// </summary>
					/// <param name="member">検索依頼を出したメンバー</param>
					/// <returns>パトロールターゲット</returns>
					std::shared_ptr<GameObject> SearchTarget(const std::shared_ptr<I_FactionMember>& member);

					//--------------------------------------------------------------------------------------
					/// アクセッサ
					//--------------------------------------------------------------------------------------

					bool IsEnd() const;

				};

			}

		}
	}
}