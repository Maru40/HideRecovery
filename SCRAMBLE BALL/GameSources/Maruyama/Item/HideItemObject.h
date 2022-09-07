/*!
@file HideItemObject.h
@brief HideItemObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StageObject/StageObjectBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �B���A�C�e���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class HideItemObject : public StageObjectBase
	{
	public:
		using DrawComp = PNTBoneModelDraw;

	private:

	public:
		HideItemObject(const std::shared_ptr<Stage>& stage);
		HideItemObject(const std::shared_ptr<Stage>& stage, const std::wstring& line);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		void SettingModel();
	};
}