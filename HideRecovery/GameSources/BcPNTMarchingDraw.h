/*!
@file BcPNTMarchingDraw.h
@brief BcPNTMarchingDraw
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

//#include "ChildrenRender.h"

namespace basecross {

	namespace Metaball {
		class ChildrenRender;
	}

	struct MarchingConstants {
		Col4 diffuseColor;
		Col4 emissiveColor;
		Col4 specularColorAndPower;

		Col4 lightDirection[3];
		Col4 lightDiffuseColor[3];
		Col4 lightSpecularColor[3];

		Vec4 eyePosition;

		Col4 fogColor;
		Vec4 fogVector;

		Mat4x4 world;
		Vec4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//汎用フラグ
		XMUINT4 activeFlg;
		//以下影
		Vec4 lightPos;
		Vec4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		int useNumSpheres;
		XMUINT3 null;

		Vec4 spheres[256];
		Vec4 colors[256];

		Mat4x4 viewProj;

		Vec4 bones[3 * 72];
	};

	struct TriTableConstants
	{
		int triTable[256][16];
	};

	//コンスタントバッファの実装
	DECLARE_DX11_CONSTANT_BUFFER(MarchingBuffer, MarchingConstants)
	DECLARE_DX11_CONSTANT_BUFFER(TriTableBuffer, TriTableConstants)


	namespace MarchingCube
	{

		class BcPNTMarchingDraw :public BcBaseDraw
		{
			struct Impl;
			unique_ptr<Impl> pImpl;

			vector<ex_weak_ptr<Metaball::ChildrenRender>> m_renders;

			std::mutex Mutex;

			//ID3D11ShaderResourceView
			ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

		public:
			BcPNTMarchingDraw(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override {}

			void OnDraw() override;

		private:

			void DrawStatic2(const MeshPrimData& data);
			void SetMarchingConstants(MarchingConstants& pointCb, const MeshPrimData& data);
			void SetTriTableConstants(TriTableConstants& triTableCb, const MeshPrimData& data);

		public:

			//アクセッサ------------------------------------------------------------------------------

			void SetRenders(const vector<ex_weak_ptr<Metaball::ChildrenRender>>& renders);
		};

	}

}


