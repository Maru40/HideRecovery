
/*!
@file HidePlaceOpener.h
@brief HidePlaceOpenerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HidePlace;

	class HidePlaceOpener : public Component
	{
		float m_range;

		std::vector<std::weak_ptr<HidePlace>> m_allHidePlaces;	//フィールドの全ての隠すオブジェクト
		std::weak_ptr<HidePlace> m_canOpenHidePlace;

		bool m_isDrawUI = false;

	public:
		HidePlaceOpener(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		float GetRange() const { return m_range; }

		void SetIsDrawUI(bool isDrawUI) { m_isDrawUI = isDrawUI; }
		bool IsDrawUI() const { return m_isDrawUI; }

		std::shared_ptr<HidePlace> GetCanOpenaHidePlace() const { return m_canOpenHidePlace.lock(); }
	};

}