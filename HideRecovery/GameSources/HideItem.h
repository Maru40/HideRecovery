
/*!
@file HideItem.h
@brief HideItem‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "ItemBase.h"

namespace basecross {

	class HideItem : public ItemBase
	{
	public:
		HideItem(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:


	};

}