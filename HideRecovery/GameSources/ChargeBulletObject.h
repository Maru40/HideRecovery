/*!
@file ChargeBulletObject.h
@brief ChargeBulletObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class ChargeBulletObject : public GameObject 
	{
	public:
		ChargeBulletObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		void SettingModel();
	};

}