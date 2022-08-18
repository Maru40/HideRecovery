/*!
@file   GoarObject.h
@brief  GoarObject
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {

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