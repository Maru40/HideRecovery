/*!
@file AstarRouteRequester.h
@brief AstarRouteRequester
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	/// AstarRoute�̃��N�G�X�g��
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester : public I_ThreadRequester
	{
	public:
		virtual ~AstarRouteRequester() = default;
	};

}