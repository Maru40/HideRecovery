/*!
@file AstarRouteRequester.h
@brief AstarRouteRequester
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	/// AstarRouteのリクエスト者
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester : public I_ThreadRequester
	{
	public:
		virtual ~AstarRouteRequester() = default;
	};

}