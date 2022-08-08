
/*!
@file Stator_Villain.h
@brief Stator_Villainなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "I_BrackBoard.h"

#include "EnemyStatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		template<class NodeType, class EnumType, class TransitionMember> class StatorBase;

		//--------------------------------------------------------------------------------------
		///	ヴィランのステーターの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct Stator_Villain_TransitionMember
		{
			float plowlingEyeRange;
			float attackRange;	//攻撃距離

			Stator_Villain_TransitionMember();
		};

		//--------------------------------------------------------------------------------------
		/// ヴィランのステーターのブラックボード
		//--------------------------------------------------------------------------------------
		struct Stator_Villain_BlackBoard
		{

		};

		//--------------------------------------------------------------------------------------
		/// ヴィランのステーター
		//--------------------------------------------------------------------------------------
		class Stator_Villain :
			public EnemyStatorBase<EnemyBase, Stator_Villain_TransitionMember>,
			public I_BrackBoard<Stator_Villain_BlackBoard>
		{
		public:
			struct Parametor;

			using TransitionMember = Stator_Villain_TransitionMember;
			using BlackBoard = Stator_Villain_BlackBoard;

		private:
			std::unique_ptr<Parametor> m_paramPtr;  //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Stator_Villain(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移条件メンバー</param>
			Stator_Villain(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member);

		private:
			void CreateNode() override;
			void CreateEdge() override;

			//--------------------------------------------------------------------------------------
			/// パラメータ設定
			//--------------------------------------------------------------------------------------

			void SettingParametor();
			void SettingPlowling();
		};

	}
}