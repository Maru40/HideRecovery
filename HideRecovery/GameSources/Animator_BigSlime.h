
/*!
@file Animator_BigSlime.h
@brief Animator_BigSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace Motion {
			struct KnockBack_Parametor;
		}

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// 巨大スライムアニメーターのステートタイプ
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,                //待機
				IdleWait,            //攻撃後の硬直など
				Walk,                //歩く
				BodyBlowPreliminary, //予備動作
				BodyBlow,            //ボディーブロー
				KnockBack,           //ノックバック
				Pressed,             //潰れる
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムアニメーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walkに遷移するスピード
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムアニメーターブラックボード
			//--------------------------------------------------------------------------------------
			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr; //ノックバックパラメータ
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;   //プレスデータのパラメータ

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// 巨大スライムアニメーター
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>, public I_BrackBoard<Animator_BlackBoard>
			{
			public:
				using StateType = Animator_StateType;
				using TransitionMember = Animator_TransitionMember;
				using BlackBoard = Animator_BlackBoard;

			private:
				BlackBoard m_blackBoard; //ブラックボード

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				Animator(const std::shared_ptr<GameObject>& objPtr);

			private:
				void CreateNode() override;
				void CreateEdge() override;

			};

		}
	}
}