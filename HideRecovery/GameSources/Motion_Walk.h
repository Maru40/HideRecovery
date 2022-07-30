
/*!
@file Motion_Walk.h
@brief Motion_Walkなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// 移動モーション
			//--------------------------------------------------------------------------------------
			class Walk : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				Walk(const std::shared_ptr<EnemyBase>& owner);

			private:
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;
			};

		}
	}
}