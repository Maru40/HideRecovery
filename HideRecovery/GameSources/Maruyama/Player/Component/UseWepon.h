
/*!
@file UseWepon.h
@brief UseWepon�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class BulletType>
	class WeponBase;

	//--------------------------------------------------------------------------------------
	/// �E�F�|�����g�p����N���X
	//--------------------------------------------------------------------------------------
	class UseWepon : public Component
	{

	public:
		UseWepon(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		
	};

}