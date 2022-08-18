#pragma once
#include "stdafx.h"

namespace basecross
{
namespace Online
{
	class OnlineMatching;
}
	class MatchStageTransitioner : public Component
	{
		std::weak_ptr<Online::OnlineMatching> m_onlineMatching;

	public:
		MatchStageTransitioner(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;
	};
}