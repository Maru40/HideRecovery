/*!
@file SlimeChildrenObject.h
@brief SlimeChildrenObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class ChildrenObject;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// スライムの子供オブジェクト
		//--------------------------------------------------------------------------------------
		class SlimeChildrenObject : public basecross::Metaball::ChildrenObject
		{
		public:
			SlimeChildrenObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};
	}
}