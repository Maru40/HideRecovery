#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	class TimeManager :public BaseSingleton<TimeManager> {
	private:
		TimeManager() {}
		~TimeManager() {}
		friend class BaseSingleton<TimeManager>;
	public:
	};
}
