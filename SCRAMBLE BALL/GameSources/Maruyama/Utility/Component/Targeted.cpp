/*!
@file Targeted.cpp
@brief Targeted�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Targeted.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώۂ̃p�����[�^
	//--------------------------------------------------------------------------------------

	Targeted_Parametor::Targeted_Parametor():
		Targeted_Parametor(0.0f)
	{}

	Targeted_Parametor::Targeted_Parametor(const float priority):
		priority(priority),
		offset(Vec3(0.0f))
	{}

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώۂ̖{��
	//--------------------------------------------------------------------------------------

	Targeted::Targeted(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr)
	{}

	Targeted::Targeted(
		const std::shared_ptr<GameObject>& objPtr,
		const Parametor& param
	):
		Component(objPtr),
		m_param(param)
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

	bool Targeted::CanTarget() const {
		//������������Ȃ��Ȃ�A�������Ń^�[�Q�b�g�Ɏw��ł���B
		if (m_canTargetFunctions.empty()) {
			return true;
		}

		//��ł��^�[�Q�b�g�w��ł��Ȃ���������������Afalse��Ԃ��B
		for (auto& canFunction : m_canTargetFunctions) {
			if (!canFunction()) {
				return false;
			}
		}

		//�S�Ă̏�����B�����Ă��邽��true
		return true;
	}

	void Targeted::AddCanTargetFunction(const std::function<bool()>& canTargetFunction) {
		m_canTargetFunctions.push_back(canTargetFunction);
	}
}