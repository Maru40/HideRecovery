/*!
@file ScaleParent.cpp
@brief ScaleParent�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ScaleParent.h"

namespace basecross {

	ScaleParent::ScaleParent(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& parent)
		:Component(objPtr), m_parent(parent)
	{}

	void ScaleParent::OnCreate() {
		if (m_parent) {
			auto parentTrans = m_parent->GetComponent<Transform>();
			m_beforeScale = parentTrans->GetWorldScale();

			auto ParWorld = parentTrans->GetWorldMatrix();
			auto parScale = ParWorld.scaleInMatrix();
			ParWorld.scaleIdentity();
			auto PosSpan = transform->GetPosition() - ParWorld.transInMatrix();
			auto QtSpan = ParWorld.quatInMatrix();
			QtSpan = bsm::inverse(QtSpan);
			bsm::Mat4x4 ParQtMat(QtSpan);
			PosSpan *= ParQtMat;

			bsm::Mat4x4 Mat = transform->GetWorldMatrix() * ParWorld;
			bsm::Vec3 Scale, Pos;
			bsm::Quat Qt;
			Mat.decompose(Scale, Qt, Pos);
			Scale = XMVectorDivide(Scale, parScale);     //Scale�͒�������
			Scale = XMVectorDivide(Scale, parScale);     //Scale�͒�������
			PosSpan = XMVectorDivide(PosSpan, parScale);
			transform->SetScale(Scale);

			m_firstScale = Scale;
		}
	}

	void ScaleParent::OnUpdate() {
		if (!m_parent) {
			return;
		}

		auto parentTrans = m_parent->GetComponent<Transform>();
		//�O�t���[����Scale���Ⴆ��
		if (parentTrans->GetWorldScale() != m_beforeScale) {
			m_beforeScale = parentTrans->GetWorldScale();

			const auto parentScale = parentTrans->GetWorldScale();
			const auto& scale = m_firstScale;

			auto resultScale = Vec3(scale.x * parentScale.x, scale.y * parentScale.y, scale.z * parentScale.z);
			transform->SetScale(resultScale);
		}
	}

}