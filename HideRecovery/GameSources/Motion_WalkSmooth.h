
/*!
@file Motion_WalkSmooth.h
@brief Motion_WalkSmoothなど
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
			/// 伸びながら移動するモーション
			//--------------------------------------------------------------------------------------
			class WalkSmooth : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				WalkSmooth(const std::shared_ptr<EnemyBase>& owner);

			private:
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;
			};

		}
	}
}