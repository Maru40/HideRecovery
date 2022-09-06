
/*!
@file Random.cpp
@brief Randomクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"
#include "Random.h"

namespace basecross {

	namespace maru {
		std::mt19937 MyRandom::m_mt(std::random_device{}());
	}

}

//endbasecross