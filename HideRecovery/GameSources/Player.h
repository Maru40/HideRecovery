/*!
@file Player.h
@brief ÉvÉåÉCÉÑÅ[Ç»Ç«
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class RotationController;

	class Player : public GameObject
	{
		ex_weak_ptr<RotationController> m_rotationController;

	public:
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
			
		}

		void OnCreate() override;
		void OnUpdate() override;

	};

}
//end basecross

