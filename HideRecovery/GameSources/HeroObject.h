/*!
@file HeroObject.h
@brief HeroObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// ヒーローオブジェクト
		//--------------------------------------------------------------------------------------
		class HeroObject : public EnemyObjectBase
		{
		public:
			using DrawComp = PNTBoneModelDraw;

		private:

		public:
			HeroObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			void SettingModel();
		};
	}
}