/*!
@file Slime_Split.h
@brief Slime_Splitなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class Pusher;
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムの分裂パラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Split_Parametor 
			{
				PressData pressData;
			};

			//--------------------------------------------------------------------------------------
			/// スライムの分裂ステート
			//--------------------------------------------------------------------------------------
			class Slime_Split : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Split_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;  //パラメータ

				std::unique_ptr<GameTimer> m_timer = nullptr; //タイム管理クラス

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				Slime_Split(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// 生成位置配列を計算して返す
				/// </summary>
				/// <returns>生成位置配列</returns>
				std::vector<Vec3> CalculateCreatePositions();

				/// <summary>
				/// スライムを生成する
				/// </summary>
				/// <param name="position">生成位置</param>
				/// <param name="forward">生成向き</param>
				/// <returns>生成したスライム</returns>
				std::shared_ptr<GameObject> CreateSlime(const Vec3& position, const Vec3& forward);
			};

		}
	}
}