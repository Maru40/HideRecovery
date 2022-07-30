#pragma once
#include "stdafx.h"

namespace basecross
{
	class FallisDestroy : public Component
	{
	public:
		Vec3 m_ObjectPosition;
		float m_DesPos;
		FallisDestroy(const std::shared_ptr<GameObject>& ptrobj, float DestroyPosition)
			:Component(ptrobj), m_ObjectPosition(0.0f, 0.0f, 0.0f), m_DesPos(DestroyPosition)
		{ }

		void OnCreate() override;
		void OnUpdate() override;
	};
}