
/*!
@file HideItem.h
@brief HideItem�Ȃ�
�S���F�ێR�T��
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