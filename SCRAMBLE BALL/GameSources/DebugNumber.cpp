
/*!
@file Number.cpp
@brief Numberクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "DebugNumber.h"

namespace basecross {

	namespace maru {

		void Number::CreateMesh() {
			float fNumber = static_cast<float>(m_number);
			float width = m_size;
			float height = -m_size;

			m_vertices = {
				{Vec3(0.0f, 0.0f, 0.0f),    m_color, Vec2(m_width * fNumber / 512.0f       , 300.0f / 512.0f)},
				{Vec3(width , 0.0f, 0.0f),  m_color, Vec2(m_width * (fNumber + 1) / 512.0f , 300.0f / 512.0f)},
				{Vec3(0.0f, height, 0.0f),  m_color, Vec2(m_width * fNumber / 512.0f       , 400.0f / 512.0f)},
				{Vec3(width, height, 0.0f), m_color, Vec2(m_width * (fNumber + 1) / 512.0f , 400.0f / 512.0f)},
			};

			m_indices = {
				0, 1, 2,
				2, 1, 3
			};
		}

		void Number::OnStart() {
			//wstring texture = L"Number_TX";
			auto obj = GetGameObject();
			auto draw = obj->AddComponent<DrawComp>();

			CreateMesh();

			//メッシュの生成
			draw->CreateOriginalMesh(m_vertices, m_indices);
			draw->SetOriginalMeshUse(true);
			draw->SetTextureResource(m_texture);

			GetGameObject()->SetAlphaActive(true);
		}

		void Number::SetNumber(const int& number) {
			m_number = number;

			// UV座標を更新する
			float fNumber = static_cast<float>(number);
			m_vertices[0].textureCoordinate = Vec2((fNumber + 0) * m_width / 512.0f, 300.0f / 512.0f);
			m_vertices[1].textureCoordinate = Vec2((fNumber + 1) * m_width / 512.0f, 300.0f / 512.0f);
			m_vertices[2].textureCoordinate = Vec2((fNumber + 0) * m_width / 512.0f, 400.0f / 512.0f);
			m_vertices[3].textureCoordinate = Vec2((fNumber + 1) * m_width / 512.0f, 400.0f / 512.0f);

			// 新しい頂点データでメッシュを更新する
			auto drawComp = GetGameObject()->GetComponent<PCTStaticDraw>();
			drawComp->UpdateVertices(m_vertices);
		}

		void Number::SetTexture(const wstring& texture) {
			m_texture = texture;

			auto draw = GetGameObject()->GetComponent<DrawComp>();
			draw->SetTextureResource(texture);
		}
	}
}

//endbasecross

