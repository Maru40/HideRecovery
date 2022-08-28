/*!
@file   GoalObject.h
@brief  GoalObject
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	enum class team::TeamType;

	//--------------------------------------------------------------------------------------
	/// �S�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class GoalObject :public StageObjectBase
	{
		team::TeamType m_team;

	public:
		GoalObject(const shared_ptr<Stage>& stage);
		GoalObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate() override;

	private:
		void SettingModel();
	};
}