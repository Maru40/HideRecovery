/*!
@file HideItemObject.h
@brief HideItemObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �B���A�C�e���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class HideItemObject : public GameObject
	{
	public:
		using DrawComp = PNTStaticDraw;

	private:

	public:
		HideItemObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		void SettingModel();

	};

}