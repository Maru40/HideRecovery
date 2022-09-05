/*!
@file FoundManager.cpp
@brief FoundManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FoundManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	見つかる者のデータ
	//--------------------------------------------------------------------------------------

	FoundManager::Data::Data()
		:Data(nullptr)
	{}

	FoundManager::Data::Data(const std::shared_ptr<GameObject>& gameObject)
		:gameObject(gameObject)
	{}

	//--------------------------------------------------------------------------------------
	///	見つかる者管理本体
	//--------------------------------------------------------------------------------------

	FoundManager::FoundManager(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr),
		m_data(Data())
	{}

	void FoundManager::OnCreate()
	{
		m_data.gameObject = GetGameObject();
	}
}

//endbasecross