
/*!
@file I_AIRequster.h
@brief I_AIRequster�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// AI�̃��N�G�X�g�����
		//--------------------------------------------------------------------------------------
		class I_AIRequester
		{
		public:
			virtual ~I_AIRequester() = default;
		};

		//--------------------------------------------------------------------------------------
		/// AI���N�G�X�g�G�x���g�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_AIRequestEvent
		{
		public:
			virtual ~I_AIRequestEvent() = default;
		};

		//--------------------------------------------------------------------------------------
		/// ���N�G�X�g�f�[�^
		//--------------------------------------------------------------------------------------
		struct AIRequestData
		{
			std::weak_ptr<I_AIRequester> requester;	//���N�G�X�g��
			I_AIRequestEvent* eventPtr;				//�����������C�x���g

			AIRequestData(const std::shared_ptr<I_AIRequester>& requester, I_AIRequestEvent* const eventPtr);

			virtual ~AIRequestData() = default;

		public:
			void NormalTest() {}

			void ConstTest() const {}
		};

	}
}