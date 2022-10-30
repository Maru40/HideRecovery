/*!
@file TestPlayerObject.h
@brief TestPlayerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class TestPlayerObject : public GameObject
	{
	public:
		using DrawComp = PNTBoneModelDraw;

	private:
		std::weak_ptr<GameObject> m_arm;	//�A�[��

	public:
		TestPlayerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnUpdate() override;

		std::shared_ptr<GameObject> GetArm() const { return m_arm.lock(); }
	};

}