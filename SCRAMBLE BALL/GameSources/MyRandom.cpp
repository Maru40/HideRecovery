
/*!
@file MyRandom.cpp
@brief MyRandom�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"
#include "MyRandom.h"

namespace basecross {

	namespace maru {
		std::mt19937 MyRandom::m_mt(std::random_device{}());
	}

}

//endbasecross