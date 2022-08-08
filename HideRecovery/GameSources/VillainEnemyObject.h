/*!
@file VillainEnemyObject.h
@brief VillainEnemyObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// ヴィランオブジェクト
		//--------------------------------------------------------------------------------------
		class VillainObject : public EnemyObjectBase
		{
		public:
			//using DrawComp = PNTBoneModelDraw;
			using DrawComp = PNTStaticModelDraw;

		private:

		public:
			VillainObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			void SettingModel();
		};
	}
}