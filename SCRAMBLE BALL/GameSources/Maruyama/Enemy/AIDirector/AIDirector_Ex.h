
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

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class I_AIRequester;

		//--------------------------------------------------------------------------------------
		/// AIDirector�̉��ǔ�
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		private:
			

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			//���߂����肢����B
			void Request(std::shared_ptr<I_AIRequester>& requester);
		};

	}
}