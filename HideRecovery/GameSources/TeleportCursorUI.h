
/*!
@file TeleportCursorUI.h
@brief TeleportCursorUIÇ»Ç«
íSìñÅFä€éRóTäÏ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class SpriteObject;
	class ButtonUI;

	class TeleportCursorUI : public Component
	{
		std::weak_ptr<SpriteObject> m_body;
		std::weak_ptr<ButtonUI> m_bottun;

	public:
		TeleportCursorUI(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

		void OnEnable() override;
		void OnDisable() override;

	private:
		void SettingBody();
		void SettingBottun();
	};

}