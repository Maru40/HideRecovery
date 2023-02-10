
/*!
@file AIDirector_Ex.h
@brief AIDirector_Ex�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

namespace basecross {

	namespace Enemy {

		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

		};

	}
}