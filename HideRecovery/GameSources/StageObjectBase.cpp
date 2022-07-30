
/*!
@file StageObjectBase.cpp
@brief StageObjectBase�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "StageObjectBase.h"

namespace basecross {

	StageObjectBase::StageObjectBase(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
		:GameObject(stage), m_name(name), m_texture(texture)
	{}

	void StageObjectBase::OnCreate() {

	}

}

//endbasecross