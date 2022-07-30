
/*!
@file StageObjectBase.cpp
@brief StageObjectBaseクラス実体
担当：丸山裕喜
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