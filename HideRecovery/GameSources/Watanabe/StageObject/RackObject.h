/*!
@file   RackObject.h
@brief  棚オブジェクトクラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class RackObject :public GameObject {
	public:
		RackObject(const shared_ptr<Stage>& stage);
		RackObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
