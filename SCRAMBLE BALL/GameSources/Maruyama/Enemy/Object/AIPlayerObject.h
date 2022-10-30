/*!
@file AIPlayerObject.h
@brief AIPlayerObject‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy
	{

		class AIPlayerObject : public EnemyObjectBase
		{
		public:
			using DrawComp = BoneModelDraw;

		private:

		public:
			AIPlayerObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

			void CreateModel() override;
		};

	}
}