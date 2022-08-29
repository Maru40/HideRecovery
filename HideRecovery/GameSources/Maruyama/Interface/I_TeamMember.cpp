/*!
@file I_TeamMember.cpp
@brief I_TeamMember�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_TeamMember.h"

#include "Maruyama/Player/Component/OwnArea.h"

namespace basecross {
	Col4 team::GetTeamColor(team::TeamType teamType) {
		switch (teamType)
		{
		case team::TeamType::Blue:
			return BLUETEAM_COLOR;
		case team::TeamType::Red:
			return REDTEAM_COLOR;
		}
		return Col4(1);
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void I_TeamMember::SetOwnArea(const std::shared_ptr<OwnArea>& area) {
		m_ownArea = area;
	}

	std::shared_ptr<OwnArea> I_TeamMember::GetOwnArea() const {
		return m_ownArea.lock();
	}
}