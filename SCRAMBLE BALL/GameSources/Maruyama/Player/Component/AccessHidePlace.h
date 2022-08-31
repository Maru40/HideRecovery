
/*!
@file AccessHidePlace.h
@brief AccessHidePlaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HidePlace;

	class AccessHidePlace : public Component
	{
		float m_range;

		std::vector<std::weak_ptr<HidePlace>> m_allHidePlaces;	//フィールドの全ての隠すオブジェクト

	public:
		AccessHidePlace(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	public:
		void Access();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		float GetRange() const { return m_range; }
	};

}