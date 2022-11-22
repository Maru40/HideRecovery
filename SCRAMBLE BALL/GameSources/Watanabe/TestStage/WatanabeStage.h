#pragma once
#include "stdafx.h"

namespace basecross {
	class DissolveAnimator;

	class WatanabeStage :public Stage {
		vector<weak_ptr<DissolveAnimator>> m_dis;
		shared_ptr<MeshResource> m_meshData;

		//ビューの作成
		void CreateViewLight();

		void CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& width, const float& height = 15.0f, const float& depth = 1.0f);
	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
