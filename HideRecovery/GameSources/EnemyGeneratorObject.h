/*!
@file EnemyGeneratorObject.h
@brief EnemyGeneratorObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		class EnemyGeneratorObject : public GameObject
		{
		public:
			EnemyGeneratorObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			void CreateLoadPosition();
		};

	}
}