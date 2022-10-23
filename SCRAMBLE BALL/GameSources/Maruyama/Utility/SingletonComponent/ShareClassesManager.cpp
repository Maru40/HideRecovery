/*!
@file ShareClassesManager.cpp
@brief ShareClassManager�̃N���X����
�S���F�ێR�T��
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