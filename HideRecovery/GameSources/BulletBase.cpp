
/*!
@file BulletBase.cpp
@brief BulletBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BulletBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラスのパラメータ
	//--------------------------------------------------------------------------------------

	BulletBase_Parametor::BulletBase_Parametor() :
		moveDirect(Vec3::Forward()),
		moveSpeed(50.0f),
		maxRange(30.0f),
		damageData(1, nullptr)
	{}

	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラス
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