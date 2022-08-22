#pragma once
#include "stdafx.h"
#include "../Interface/I_Performable.h"

namespace basecross {
	class MessageUI :public GameObject, public I_Performable {
	public:
		MessageUI(const shared_ptr<Stage>& stage);

		//void OnCreate()override;
	};
}
