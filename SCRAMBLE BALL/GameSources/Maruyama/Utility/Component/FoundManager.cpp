/*!
@file FoundManager.cpp
@brief FoundManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FoundManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	������҂̃f�[�^
	//--------------------------------------------------------------------------------------

	FoundManager::Data::Data()
		:Data(nullptr)
	{}

	FoundManager::Data::Data(const std::shared_ptr<GameObject>& gameObject)
		:gameObject(gameObject)
	{}

	//--------------------------------------------------------------------------------------
	///	������ҊǗ��{��
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