/*
@brief Gunjiゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class SinScaling : public Component
	{
		Vec3 m_scale;
		float m_delta;
		float m_deltaSpeed;
		float m_minScale;

		Vec3 m_firstScale;

		float m_scaleAdjust;

	public:
		SinScaling(const std::shared_ptr<GameObject>& gameObjectPtr)
			:Component(gameObjectPtr), m_delta(0.0f), m_deltaSpeed(2.0f),m_minScale(1.0f),
			m_firstScale(0.0f),m_scaleAdjust(0.25f)
		{}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void OnDraw() {};

		void Reset();

		void SetScaleAdjust(const float scale) {
			m_scaleAdjust = scale;
		}

		void SetSpeed(const float speed) {
			m_deltaSpeed = speed;
		}

	protected:
		void Scaling();
	};
}