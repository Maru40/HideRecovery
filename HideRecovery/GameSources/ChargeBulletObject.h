/*!
@file ChargeBulletObject.h
@brief ChargeBulletObject‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Bullet/Object/BulletObjectBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	class ChargeBulletObject : public BulletObjectBase
	{
		std::wstring m_teamTypeStr;
	public:
		ChargeBulletObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnDestroy() override;

		void SetTeamType(team::TeamType teamType);
	private:
		void SettingEffect();
	};
}