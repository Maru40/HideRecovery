#pragma once
#include "stdafx.h"
#include "../Utility/AdvRenderer.h"

namespace basecross {
	struct AdvStageConstruct
	{
		Mat4x4 view;
		Mat4x4 projection;
		Mat4x4 inverseViewProj;
		float viewNear;
		float viewFar;
		Vec2 viewPort;
		Col4 lightAmbient;
	};

	DECLARE_DX11_CONSTANT_BUFFER(LPPStageCB, AdvStageConstruct)

	class AdvStageBase :public Stage {
		static shared_ptr<AdvRenderer> m_advRender;

		Col4 m_lightAmbient = Col4(0.1f, 0.1f, 0.1f, 1.0f);

		set<int> m_drawLayers;

		//Spriteかそうでないかを分離する配列
		vector<shared_ptr<GameObject>> m_spriteVec;
		vector<shared_ptr<GameObject>> m_object3DVec;
		//3Dの透明と非透明を分離する配列
		vector<shared_ptr<GameObject>> m_object3DNormalVec;
		vector<shared_ptr<GameObject>> m_object3DAlphaVec;

		void UpdateStageCB();
		void DrawPostProcess();
	public:
		//構築と破棄
		AdvStageBase() :Stage() {}
		virtual ~AdvStageBase() {}

		void SetLightAmbient(const Col4& lightAmbient) { m_lightAmbient = lightAmbient; }
		void SetLightAmbient(float r, float g, float b, float a) { SetLightAmbient(Col4(r, g, b, a)); }
		Col4 GetLightAmbient() const { return m_lightAmbient; }

		static const shared_ptr<AdvRenderer>& GetAdvRenderer();

		void RenderStage() override;
		void DrawStage() override;
		void DrawSprite();
	};
}
