/*!
@file ChargeBulletObject.h
@brief ChargeBulletObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Bullet/Object/BulletObjectBase.h"

namespace basecross {
	class ChargeBulletObject : public BulletObjectBase
	{
	public:
		ChargeBulletObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnDestroy() override;

	private:
		void SettingModel();
	};
}