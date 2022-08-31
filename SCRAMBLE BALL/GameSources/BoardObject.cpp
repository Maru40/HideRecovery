
/*!
@file BoardObject.cpp
@brief BoardObjクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BoardObject.h"

#include "BuilderVertexPCT.h"

namespace basecross {

	using namespace Builder;

	BoardObject::BoardObject(const std::shared_ptr<Stage>& stage,
		const Builder::VertexPCTParametor& param
	):
		GameObject(stage),m_paramPtr(new Builder::VertexPCTParametor(param))
	{}

	void BoardObject::OnCreate()
	{
		auto builder = BuilderVertexPCT(*m_paramPtr.get());

		auto drawComp = AddComponent<BcPCTStaticDraw>();
		drawComp->CreateOriginalMesh(builder.m_vertices, builder.m_indices);
		drawComp->SetOriginalMeshUse(true);
		if (m_paramPtr->texture != L"") {
			drawComp->SetTextureResource(m_paramPtr->texture);
		}
		drawComp->SetDepthStencilState(DepthStencilState::Read);
		//drawComp->SetBlendState(BlendState::Additive);

		SetAlphaActive(true);
	}

	const std::shared_ptr<const Builder::VertexPCTParametor> BoardObject::GetVertexPCTParametor() const {
		return m_paramPtr;
	}

	void BoardObject::SetDiffuse(const Col4& color) {
		auto drawComp = GetComponent<BcPCTStaticDraw>();
		drawComp->SetDiffuse(color);
	}

	Col4 BoardObject::GetDiffuse() const {
		return GetComponent<BcPCTStaticDraw>()->GetDiffuse();
	}
}

//endbasecross