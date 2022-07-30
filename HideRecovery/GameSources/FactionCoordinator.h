
/*!
@file FactionCoordinator.h
@brief FactionCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		class AIDirector;

		//�O���[�v�����A�����o�[�̃A�T�C��
		class FactionCoordinator
		{
		private:
			ex_weak_ptr<AIDirector> m_director;



		public:
			FactionCoordinator(const std::shared_ptr<AIDirector>& objPtr);

		};

	}
}