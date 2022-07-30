/*!
@file Targeted.cpp
@brief Targetedなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "Targeted.h"

namespace basecross {

	Targeted::Targeted(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr)
	{}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	Vec3 Targeted::GetPosition() const {
		return transform->GetPosition() + m_param.offset;
	}

	void Targeted::SetOffset(const Vec3& offset) {
		m_param.offset = offset;
	}

	Vec3 Targeted::GetOffset() const {
		return m_param.offset;
	}
}