/*!
@file Task_BodyBlow_BigSlime.h
@brief Task_BodyBlow_BigSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "TaskList.h"

namespace basecross {

	namespace Enemy {
		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// 巨大スライムのボディブローのパラメーター
				//--------------------------------------------------------------------------------------
				struct BodyBlow_Parametor {
					Task_BodyBlow::Parametor sumAbsorbedParametor; //Absorbedのパラメータの合計をカウント

					BodyBlow_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// 巨大スライムのボディブロータスク
				//--------------------------------------------------------------------------------------
				class BodyBlow : public TaskNodeBase<EnemyBase>
				{
				public:
					using Parametor = BodyBlow_Parametor;

				private:
					ex_weak_ptr<Parametor> m_paramPtr;  //パラメータ
					bool m_isActive = false;            //このノードのアクティブ状態を表す。

				public:
					/// <summary>
					/// コンストラクタ
					/// </summary>
					/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
					/// <param name="paramPtr">パラメータ</param>
					BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// 移動更新
					/// </summary>
					void MoveUpdate();

					/// <summary>
					/// 向き更新
					/// </summary>
					void Rotation();

					/// <summary>
					/// 移動した距離を計測する
					/// </summary>
					void ElapsedMoveRangeUpdate();

					/// <summary>
					/// 当たったときのダメ―ジを与えるなどの処理
					/// </summary>
					/// <param name="pair">当たったデータ</param>
					void CollisionEnter(const CollisionPair& pair);

					/// <summary>
					/// ダメージ情報を当たった相手に送る
					/// </summary>
					/// <param name="pair"></param>
					void SendDamage(const CollisionPair& pair);

					/// <summary>
					/// 終了判断
					/// </summary>
					/// <returns>更新終了したならtrue</returns>
					bool IsEnd();
				};

			}
		}
	}
}