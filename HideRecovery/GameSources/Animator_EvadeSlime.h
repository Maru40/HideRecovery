
/*!
@file Animator_EvadeSlime.h
@brief Animator_EvadeSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace Motion {
			struct KnockBack_Parametor;
		}

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムのアニメーターステートタイプ
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,      //待機
				IdleWait,  //攻撃後の硬直など
				KnockBack, //ノックバック
				Pressed,   //潰される
				Walk,      //歩行
			};

			//--------------------------------------------------------------------------------------
			/// アニメーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walkに遷移するスピード
			};

			//--------------------------------------------------------------------------------------
			/// アニメーターのブラックボード
			//--------------------------------------------------------------------------------------
			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr; //ノックバックパラメータ
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;   //潰される時のパラメータ

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムのアニメータ
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>
			{
			public:
				using StateType = Animator_StateType;
				using TransitionMember = Animator_TransitionMember;
				using BlackBoard = Animator_BlackBoard;

			private:
				BlackBoard m_blackBoard;   //ブラックボード

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				Animator(const std::shared_ptr<GameObject>& objPtr);

			private:
				void CreateNode() override;
				void CreateEdge() override;

			public:

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// ブラックボードの設定
				/// </summary>
				/// <param name="blackBoard">ブラックボード</param>
				void SetBlackBoard(const BlackBoard& blackBoard);

				/// <summary>
				/// ブラックボードの取得
				/// </summary>
				/// <returns>ブラックボード</returns>
				BlackBoard GetBlackBoard() const;

				/// <summary>
				/// ブラックボードの参照を取得
				/// </summary>
				/// <returns>ブラックボードの参照</returns>
				BlackBoard& GetRefBlackBoard();

			};

		}
	}
}