
/*!
@file BulletObjectBase.h
@brief BulletObjectBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class BulletObjectBase : public GameObject
	{
	public:
		BulletObjectBase(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{}
	};

}