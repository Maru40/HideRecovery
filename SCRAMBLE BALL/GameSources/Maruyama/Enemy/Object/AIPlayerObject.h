/*!
@file AIPlayerObject.h
@brief AIPlayerObject�Ȃ�
�S���F�ێR�T��
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
			AIPlayerObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};

	}
}