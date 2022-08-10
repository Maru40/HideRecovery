/*!
@file HideItemObject.h
@brief HideItemObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �B���A�C�e���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class HideItemObject : public StageObjectBase
	{
	public:
		using DrawComp = PNTStaticDraw;

	private:

	public:
		HideItemObject(const std::shared_ptr<Stage>& stage, const std::wstring& name);

		void OnCreate() override;

	private:
		void SettingModel();

	};

}