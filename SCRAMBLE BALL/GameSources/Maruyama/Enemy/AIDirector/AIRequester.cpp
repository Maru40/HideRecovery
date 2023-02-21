
/*!
@file AIRequester.cpp
@brief AIRequester�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AIRequester.h"

namespace basecross {

	namespace Enemy 
	{
		//--------------------------------------------------------------------------------------
		/// ���N�G�X�g�f�[�^�{��
		//--------------------------------------------------------------------------------------

		AIRequestData::AIRequestData(
			const std::shared_ptr<I_AIRequester>& requester,
			I_AIRequestEvent* const eventPtr
		) :
			requester(requester),
			eventPtr(eventPtr)
		{}
	}
}