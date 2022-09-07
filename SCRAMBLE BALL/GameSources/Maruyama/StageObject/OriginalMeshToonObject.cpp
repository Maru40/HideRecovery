
/*!
@file OriginalMeshStageObject.cpp
@brief OriginalMeshStageObjectクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"

#include "OriginalMeshToonObject.h"

#include "Maruyama/Load/StageMapCSV.h"

#include "Maruyama/Shader/Toon/ToonPNTStaticModelDraw.h"

#include "Maruyama/Utility/Utility.h"

#include "Maruyama/Utility/Object/CollisionObject.h"

namespace basecross {

	OriginalMeshToonObject::OriginalMeshToonObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
		:StageObjectBase(stage, name, texture)
	{}

	void OriginalMeshToonObject::OnCreate() {
		StageObjectBase::OnCreate();

		m_scaleOffset = Vec3(0.25f);
		//メッシュの調整用Mat
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(m_scaleOffset),
			Vec3(0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		auto draw = AddComponent<ToonPNTStaticModelDraw>();
		draw->SetMeshResource(GetName());
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetDiffuse(Col4(0.3f));

		//影付け
		auto shadow = AddComponent<Shadowmap>();
		shadow->SetMeshResource(GetName());
		shadow->SetMeshToTransformMatrix(spanMat);

		auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
		transform->SetScale(Vec3(offset));

		SetAlphaActive(true);

		AddTag(L"T_Obstacle");
	}

	void OriginalMeshToonObject::OnUpdate() {
		if (!m_isStart) {
			auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
			auto scale = Vec3(transform->GetScale() * offset);
			transform->SetScale(scale);

			CreateCollisionObject(transform->GetScale());

			m_isStart = true;
		}
	}

	void OriginalMeshToonObject::CreateCollisionObject(const Vec3& scale) {
		if (!m_isCollision) {
			return;
		}

		auto param = CollisionObjectManager::Parametor();
		param.objName = m_name;
		param.folderName = L"MapCollisionOffsetData/";
		param.fileName = L"MapCollisionOffsetData.csv";
		param.baseScale = scale;

		auto col = AddComponent<CollisionObjectManager>(param);
		//col->SetCollisionDrawActive(true);
	}

}

//endbasecrosss