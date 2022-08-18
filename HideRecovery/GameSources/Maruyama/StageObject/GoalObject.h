/*!
@file   GoalObject.h
@brief  GoalObject
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class GoalObject :public StageObjectBase 
	{
	public:
		GoalObject(const shared_ptr<Stage>& stage);
		GoalObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate() override;

	private:
		void SettingModel();
	};

}