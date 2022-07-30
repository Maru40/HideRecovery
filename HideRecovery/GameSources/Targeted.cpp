/*!
@file Targeted.cpp
@brief Targeted�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Targeted.h"

namespace basecross {

	Targeted::Targeted(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr)
	{}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
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