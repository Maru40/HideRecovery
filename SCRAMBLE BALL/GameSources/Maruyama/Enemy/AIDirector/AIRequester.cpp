
/*!
@file AIRequester.cpp
@brief AIRequesterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AIRequester.h"

namespace basecross {

	namespace Enemy 
	{
		//--------------------------------------------------------------------------------------
		/// リクエストデータ本体
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