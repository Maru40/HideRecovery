/*!
@file I_TeamMember.cpp
@brief I_TeamMemberクラス実体
担当：丸山裕喜
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

	wstring team::GetTeamTypeString(team::TeamType teamType) {
		switch (teamType)
		{
		case team::TeamType::Blue:
			return L"Blue";
		case team::TeamType::Red:
			return L"Red";
		}
		// デフォルト値
		return L"Blue";
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void I_TeamMember::SetOwnArea(const std::shared_ptr<OwnArea>& area) {
		m_ownArea = area;
	}

	std::shared_ptr<OwnArea> I_TeamMember::GetOwnArea() const {
		return m_ownArea.lock();
	}
}