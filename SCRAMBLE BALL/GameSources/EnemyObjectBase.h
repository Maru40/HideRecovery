/*!
@file EnemyObjectBase.h
@brief EnemyObjectBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		/// 敵オブジェクトの基底クラス
		//--------------------------------------------------------------------------------------
		class EnemyObjectBase : public GameObject
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			EnemyObjectBase(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};
	}
}

//endbasecross