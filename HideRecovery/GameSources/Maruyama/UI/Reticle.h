
/*!
@file Reticle.h
@brief Reticle�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class SpriteObject;

	class Reticle : public Component
	{
		std::weak_ptr<SpriteObject> m_sprite;

	public:
		Reticle(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
	};

}