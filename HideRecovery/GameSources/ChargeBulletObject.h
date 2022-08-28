/*!
@file ChargeBulletObject.h
@brief ChargeBulletObject‚È‚Ç
’S“–FŠÛR—TŠì
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