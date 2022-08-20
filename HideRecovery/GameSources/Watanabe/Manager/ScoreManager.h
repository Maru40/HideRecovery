#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	class ScoreManager :public BaseSingleton<ScoreManager> {
	private:
		ScoreManager() {}
		~ScoreManager() {}
		friend class BaseSingleton<ScoreManager>;
	public:
	};
}
