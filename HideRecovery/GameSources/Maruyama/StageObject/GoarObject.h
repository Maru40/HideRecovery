/*!
@file   GoarObject.h
@brief  GoarObject
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class GoarObject :public StageObjectBase 
	{
	public:
		GoarObject(const shared_ptr<Stage>& stage);
		GoarObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate() override;

	private:
		void SettingModel();
	};

}