/*!
@file AuraBase.cpp
@brief AuraBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AuraBase.h"

#include "TimeHelper.h"

namespace basecross {
	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// オーラの更新パラメータ
		//--------------------------------------------------------------------------------------

		Aura_UpdateParametor::Aura_UpdateParametor():
			Aura_UpdateParametor(0.75f, Vec2(1.0f, 0.0f))
		{}

		Aura_UpdateParametor::Aura_UpdateParametor(const float& speed, const Vec2& direct):
			speed(speed), direct(direct)
		{}

		//--------------------------------------------------------------------------------------
		/// オーラのパラメータ
		//--------------------------------------------------------------------------------------

		Aura_Parametor::Aura_Parametor():
			Aura_Parametor(
				20, 2.5f,
				1.0f, 2.0f,
				Col4(1.0f), 
				L"Aura_TX",
				Aura_UpdateParametor()
			)
		{}

		Aura_Parametor::Aura_Parametor(
			const int& sides, const float& height,
			const float& topRadius, const float& bottomRadius,
			const Col4& color, 
			const wstring& texture,
			const Aura_UpdateParametor& updateParam
		):
			sides(sides), height(height),
			topRadius(topRadius), bottomRadius(bottomRadius),
			topColor(color), bottomColor(color),
			texture(texture),
			updateParam(updateParam),
			blendStage(BlendState::Additive)
		{}

		//--------------------------------------------------------------------------------------
		/// オーラの基底クラス
		//--------------------------------------------------------------------------------------

		AuraBase::AuraBase(const std::shared_ptr<GameObject>& objPtr)
			: AuraBase(objPtr, Parametor())
		{}

		AuraBase::AuraBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
			: Component(objPtr), m_param(parametor)
		{}

		void AuraBase::OnCreate() {
			//頂点の生成
			std::vector<VertexPositionColorTexture> vertices = CreateVertices();
			m_vertices = vertices;  //メンバにセット

			//インデックスの生成
			std::vector<uint16_t> indices = CreateIndices();

			//コンポーネントの設定
			auto drawComp = GetGameObject()->AddComponent<DrawComp>();
			drawComp->CreateOriginalMesh(vertices, indices);
			drawComp->SetOriginalMeshUse(true);
			drawComp->SetTextureResource(m_param.texture);
			drawComp->SetBlendState(m_param.blendStage);
			drawComp->SetDepthStencilState(DepthStencilState::Read);
			drawComp->SetSamplerState(SamplerState::AnisotropicWrap);

			GetGameObject()->SetAlphaActive(true);
		}

		void AuraBase::OnUpdate() {
			UpdateUV();
		}

		void AuraBase::UpdateUV() {
			auto delta = App::GetApp()->GetElapsedTime();
			const auto& updateParam = m_param.updateParam;
			const float& speed = updateParam.speed;
			const Vec2& direct = updateParam.direct.GetNormalize();

			for (auto& vertex : m_vertices) {
				vertex.textureCoordinate += direct * delta * speed;
			}

			auto drawComp = GetGameObject()->GetComponent<DrawComp>();
			drawComp->UpdateVertices(m_vertices);
		}

		std::vector<VertexPositionColorTexture> AuraBase::CreateVertices() {
			std::vector<VertexPositionColorTexture> vertices = {};

			//パラメータの参照
			const int& sides = m_param.sides;
			const float& height = m_param.height;
			const float& topRadius = m_param.topRadius;
			const float& bottomRadius = m_param.bottomRadius;
			const Col4& topColor = m_param.topColor;
			const Col4& bottomColor = m_param.bottomColor;

			//バーテックスの生成
			for (int i = 0; i < sides + 1; i++)
			{
				float fSides = static_cast<float>(sides);
				const float radian = XMConvertToRadians(i * 360.0f / fSides);
				auto topPosition = Vec3(topRadius * cosf(radian), height, topRadius * sinf(radian));
				auto bottomPosition = Vec3(bottomRadius * cosf(radian), 0.0f, bottomRadius * sinf(radian));
				vertices.push_back({ topPosition,    topColor,    Vec2(i / fSides, 0.0f) }); // 0
				vertices.push_back({ bottomPosition, bottomColor, Vec2(i / fSides, 1.0f) }); // 1
			}

			return vertices;
		}

		std::vector<uint16_t> AuraBase::CreateIndices() {
			std::vector<uint16_t> indices = {};
			const int& sides = m_param.sides;

			// ループ処理のベースにするインデックス
			const uint16_t BaseIndices[] = { 
				1, 0, 2,
				1, 2, 3,
			};

			//インデックスの生成
			for (int i = 0; i < sides; i++) {
				for (auto baseIndex : BaseIndices) {
					indices.push_back(baseIndex + 2 * i); // ベースインデックスを２ずつずらして設定していく
				}
			}

			return indices;
		}

	}
}