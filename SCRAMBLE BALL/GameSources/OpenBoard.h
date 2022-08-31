
/*!
@file ItemAcquisitionManager.h
@brief ItemAcquisitionManager‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class BoardObject;
	class ButtonUI;

	class OpenBoard : public Component
	{
		std::weak_ptr<BoardObject> m_stringSprite;
		std::weak_ptr<BoardObject> m_button;

	public:
		OpenBoard(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

		void OnEnable() override;
		void OnDisable() override;

	private:
		void CreateStringSprite();
		void CreateButton();
	};

}