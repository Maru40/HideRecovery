
/*!
@file AIDirector_Ex.cpp
@brief AIDirector_ExÌNXÀÌ
SFÛRTì
*/

#include "stdafx.h"
#include "Project.h"

#include "AIDirector_Ex.h"

#include "TupleSpace.h"
#include "AIRequester.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	std::weak_ptr<Enemy::AIDirector_Ex> maru::SingletonComponent<Enemy::AIDirector_Ex>::sm_instance;

	namespace Enemy {

		AIDirector_Ex::AIDirector_Ex(const std::shared_ptr<GameObject>& owner) :
			SingletonComponent(owner),
			m_threadPool(new ThreadPool())
		{}

		

	}

}