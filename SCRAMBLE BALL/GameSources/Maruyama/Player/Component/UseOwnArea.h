
/*!
@file UseOwnArea.h
@brief UseOwnArea�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���w�G���A���g�p����N���X
	//--------------------------------------------------------------------------------------
	class UseOwnArea : public Component
	{
	public:
		UseOwnArea(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
	};

}