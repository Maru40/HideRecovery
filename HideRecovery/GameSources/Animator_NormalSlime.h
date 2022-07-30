
/*!
@file Animator_NormalSlime.h
@brief Animator_NormalSlimeなど
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

		namespace Motion{
			struct KnockBack_Parametor;
		}

		namespace NormalSlime {

			//--------------------------------------------------------------------------------------
			/// 通常スライムのアニメーターステートタイプ
			//--------------------------------------------------------------------------------------
			enum class Animator_StateType
			{
				Idle,                 //待機
				IdleWait,             //攻撃後の硬直など
				Walk,                 //歩行
				BodyBlowPreliminary,  //予備動作
				BodyBlow,             //ボディ―ブロー
				KnockBack,            //ノックバック
				Pressed,              //潰される
				Max,                  //Enumの最後
			};

			//--------------------------------------------------------------------------------------
			/// 通常スライムのアニメーターの遷移条件メンバー
			//--------------------------------------------------------------------------------------
			struct Animator_TransitionMember
			{
				float toWalkSpeed = 0.2f;  //walkに遷移するスピード
			};

			struct Animator_BlackBoard {
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> knockBackParamPtr;  //ノックバックパラメータ
				std::shared_ptr<Enemy::Motion::KnockBack_Parametor> pressedParamPtr;    //潰されるパラメータ

				Animator_BlackBoard();
			};

			//--------------------------------------------------------------------------------------
			/// 通常スライムのアニメーター
			//--------------------------------------------------------------------------------------
			class Animator : public AnimatorBase<EnemyBase, Animator_StateType, Animator_TransitionMember>
			{
			public:
				using StateType = Animator_StateType;
				using TransitionMember = Animator_TransitionMember;
				using BlackBoard = Animator_BlackBoard;

			private:
				//ブラックボード
				BlackBoard m_blackBoard;

			public :
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
				void SetBlackBoard(const BlackBoard& blackBoard) noexcept;

				/// <summary>
				/// ブラックボードの取得
				/// </summary>
				/// <returns>ブラックボード</returns>
				BlackBoard GetBlackBoard() const noexcept;

				/// <summary>
				/// ブラックボードの参照を取得
				/// </summary>
				/// <returns>ブラックボードの参照</returns>
				BlackBoard& GetRefBlackBoard();

			};

		}
	}
}