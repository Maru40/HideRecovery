/*!
@file ShareClassesManager.cpp
@brief ShareClassManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ShareClassesManager.h"

namespace basecross {

	std::weak_ptr<ShareClassesManager> maru::SingletonComponent<ShareClassesManager>::sm_instance;

	ShareClassesManager::ShareClassesManager(std::shared_ptr<GameObject>& objPtr) :
		SingletonComponent(objPtr)
	{}

}