/*!
@file MetaballChildrenObject.h
@brief MetaballChildrenObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	メタボールの子供オブジェクト
		//--------------------------------------------------------------------------------------
		class ChildrenObject : public GameObject
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			ChildrenObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};

	}
}

//endbasecross