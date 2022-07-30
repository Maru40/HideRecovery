/*!
@file Slime_Find.h
@brief Slime_Findなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムの敵を見つけた時のステート
			//--------------------------------------------------------------------------------------
			class Slime_Find : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				Slime_Find(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override {};
				bool OnUpdate() override { return true; };
				void OnExit() override {};
 			};

		}
	}
}
