
/*!
@file BulletBase.cpp
@brief BulletBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "BulletBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�e�ۂ̊��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------

	BulletBase_Parametor::BulletBase_Parametor() :
		moveDirect(Vec3::Forward()),
		moveSpeed(30.0f),
		maxRange(30.0f)
	{}

	//--------------------------------------------------------------------------------------
	///	�e�ۂ̊��N���X
	//--------------------------------------------------------------------------------------

	BulletBase::BulletBase(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void BulletBase::OnUpdate() {
		MoveUpdate();
	}

	void BulletBase::MoveUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();

		auto position = transform->GetPosition();
		position += GetMoveDirect() * m_param.moveSpeed * delta;
		transform->SetPosition(position);
	}
}