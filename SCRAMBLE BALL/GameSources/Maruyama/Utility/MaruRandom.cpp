
/*!
@file Random.cpp
@brief Random�N���X����
�S���F�ێR�T��
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