/*!
@file AIPlayerObject.h
@brief AIPlayerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

#include "Itabashi/OtherPlayerObject.h"

namespace basecross {
	namespace Enemy
	{

		class AIPlayerObject : public StageObject::OtherPlayerObject
		{
		public:
			using DrawComp = BoneModelDraw;

		private:

		public:
			AIPlayerObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

			void CreateModel();
		};

	}
}