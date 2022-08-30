
/*!
@file TeleportUI.h
@brief TeleportUIÇ»Ç«
íSìñÅFä€éRóTäÏ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class SpriteObject;
	class ButtonUI;

	class TeleportUI : public Component
	{
		std::weak_ptr<SpriteObject> m_body;
		std::weak_ptr<ButtonUI> m_bottun;

		std::weak_ptr<SoundEmitter> m_soundEmitter;
		SoundClip m_soundClip;

	public:
		TeleportUI(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

		void OnEnable() override;
		void OnDisable() override;

	private:
		void SettingBody();
		void SettingBottun();
	};

}