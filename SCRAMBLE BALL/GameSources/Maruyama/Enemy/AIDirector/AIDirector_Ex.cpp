
/*!
@file AIDirector_Ex.cpp
@brief AIDirector_Exのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AIDirector_Ex.h"

#include "TupleSpace.h"
#include "AIRequester.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Maruyama/Enemy/Astar/AstarThreadController.h"

namespace basecross {

	std::weak_ptr<Enemy::AIDirector_Ex> maru::SingletonComponent<Enemy::AIDirector_Ex>::sm_instance;

	namespace Enemy {

		AIDirector_Ex::AIDirector_Ex(const std::shared_ptr<GameObject>& owner) :
			SingletonComponent(owner),
			m_astarThread(new AstarThreadController(1))
		{}

		_NODISCARD const std::unique_ptr<AstarThreadController>& AIDirector_Ex::GetAstarThreadController() const { return m_astarThread; }

	}

}