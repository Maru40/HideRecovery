
/*!
@file AIPlayerStator.h
@brief AIPlayerStatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StatorBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	

	namespace Enemy {

		class EnemyBase;

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorのステートタイプ
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//隠し場所探す。
			Attack,				//攻撃
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorの遷移条件メンバー
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {

		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator
		//--------------------------------------------------------------------------------------
		class AIPlayerStator : StatorBase<EnemyBase, AIPlayerStator_StateType, AIPlayerStator_TransitionMember>
		{
		public:
			using StateType = AIPlayerStator_StateType;
			using TransitionMember = AIPlayerStator_TransitionMember;

		private:


		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			void CreateNode() override;
			void CreateEdge() override;

		};

	}
}