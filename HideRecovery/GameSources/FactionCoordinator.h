
/*!
@file FactionCoordinator.h
@brief FactionCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class AIDirector;

		//--------------------------------------------------------------------------------------
		/// �O���[�v�����A�����o�[�̃A�T�C��
		//--------------------------------------------------------------------------------------
		class FactionCoordinator
		{
		private:
			std::weak_ptr<AIDirector> m_director;

		public:
			FactionCoordinator(const std::shared_ptr<AIDirector>& objPtr);

			void OnStart();
			void OnUpdate();
			void OnExit();

		private:
			

		public:
			/// <summary>
			/// �f�B���N�^�[�̎擾
			/// </summary>
			/// <returns>�f�B���N�^�[</returns>
			std::shared_ptr<AIDirector> GetDirector() const { return m_director.lock(); }

		};

	}
}