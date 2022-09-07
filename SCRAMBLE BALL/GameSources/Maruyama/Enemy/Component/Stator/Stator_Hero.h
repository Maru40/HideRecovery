
/*!
@file Stator_Hero.h
@brief Stator_Heroなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Patch/Trigger.h"
#include "Maruyama/Interface/I_BrackBoard.h"

#include "EnemyStatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		template<class NodeType, class EnumType, class TransitionMember> class StatorBase;

		//--------------------------------------------------------------------------------------
		///	ヒーローのステーターの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct Stator_Hero_TransitionMember
		{
			float plowlingEyeRange;	//徘徊時視界
			float attackRange;		//攻撃距離

			Stator_Hero_TransitionMember();
		};

		//--------------------------------------------------------------------------------------
		/// ヒーローのステーターのブラックボード
		//--------------------------------------------------------------------------------------
		struct Stator_Hero_BlackBoard
		{
			
		};

		//--------------------------------------------------------------------------------------
		/// ヒーローのステーター
		//--------------------------------------------------------------------------------------
		class Stator_Hero : 
			public EnemyStatorBase<EnemyBase, Stator_Hero_TransitionMember>,
			public I_BrackBoard<Stator_Hero_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_Hero_TransitionMember;
			using BlackBoard = Stator_Hero_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Stator_Hero(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移条件メンバー</param>
			Stator_Hero(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

		private:
			void CreateNode() override;
			void CreateEdge() override;

			//--------------------------------------------------------------------------------------
			/// 遷移条件設定
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 視界内にターゲットがいるかどうか
			/// </summary>
			/// <param name="member">遷移条件メンバー</param>
			/// <returns></returns>
			bool IsInEyeRangeTarget(const TransitionMember& member);

			//--------------------------------------------------------------------------------------
			/// パラメータ設定
			//--------------------------------------------------------------------------------------

			void SettingParametor();
			void SettingPlowling();
			void SettingChase();
		};

	}
}