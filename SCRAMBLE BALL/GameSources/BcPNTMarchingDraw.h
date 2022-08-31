/*!
@file BcPNTMarchingDraw.h
@brief BcPNTMarchingDraw
�S���ҁF�ێR �T��
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
		//�ėp�t���O
		XMUINT4 activeFlg;
		//�ȉ��e
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

	//�R���X�^���g�o�b�t�@�̎���
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

			//�A�N�Z�b�T------------------------------------------------------------------------------

			void SetRenders(const vector<ex_weak_ptr<Metaball::ChildrenRender>>& renders);
		};

	}

}


