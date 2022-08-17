
/*!
@file OwnArea.cpp
@brief OwnArea�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OwnArea.h"

#include "Maruyama/Interface/I_TeamMember.h"

#include "MaruUtility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���w�G���A�̃p�����[�^
	//--------------------------------------------------------------------------------------

	OwnArea_Parametor::OwnArea_Parametor(const Team& team, const float radius):
		team(team),
		radius(radius)
	{}

	//--------------------------------------------------------------------------------------
	/// ���w�G���A�{��
	//--------------------------------------------------------------------------------------

	OwnArea::OwnArea(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr),
		m_param(parametor)
	{}

	void OwnArea::OnCreate() {
		
	}

	bool OwnArea::IsInArea(const std::shared_ptr<I_TeamMember>& member) {
		return IsInArea(member->GetOwnerObject());
	}

	bool OwnArea::IsInArea(const std::shared_ptr<GameObject>& member) {
		auto toObjectVec = maru::Utility::CalcuToTargetVec(GetGameObject(), member);
		return toObjectVec.length() < GetRadius();	//���a���Ȃ�true
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void OwnArea::AddMember(const std::shared_ptr<I_TeamMember>& member) {
		member->SetOwnArea(GetThis<OwnArea>());
		m_members.push_back(member);
	}

	bool OwnArea::RemoveMember(const std::shared_ptr<I_TeamMember>& member) {
		auto iter = m_members.begin();
		while (iter != m_members.end()) {
			if ((*iter).lock() == member) {
				iter = m_members.erase(iter);
				return true;
			}
			iter++;
		}

		return false;
	}

}