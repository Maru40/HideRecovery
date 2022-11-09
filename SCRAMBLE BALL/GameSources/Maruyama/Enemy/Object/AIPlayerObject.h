/*!
@file AIPlayerObject.h
@brief AIPlayerObject‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

#include "Maruyama/Player/Object/PlayerObject.h"
#include "Maruyama/Player/Object/VillainPlayerObject.h"

namespace basecross {
	namespace Enemy
	{

		class AIPlayerObject : public PlayerObject
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