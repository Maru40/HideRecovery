
/*!
@file Slime_Union.h
@brief Slime_Unionなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムの合体パラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Union_Parametor
			{
				float time = 1.0f;  //Unionに掛ける時間
			};

			//--------------------------------------------------------------------------------------
			/// スライムの合体ステート
			//--------------------------------------------------------------------------------------
			class Slime_Union : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Union_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //パラメータ

				std::unique_ptr<GameTimer> m_timer;  //タイム管理クラス
				
				Vec3 m_startPosition;                //ステート開始位置
				
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				Slime_Union(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// ターゲットに自分のメタボールを譲渡する。
				/// </summary>
				void TranserChildren();

				/// <summary>
				/// 合体アップデート
				/// </summary>
				void UpdateUnion();

				/// <summary>
				/// 合体成功処理
				/// </summary>
				void SccessUnion();

				/// <summary>
				/// 合体失敗処理
				/// </summary>
				void MissUnion();
			};

		}
	}

}