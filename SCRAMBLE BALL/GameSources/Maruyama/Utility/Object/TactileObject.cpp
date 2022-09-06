/*!
@file TactileObject.cpp
@brief TactileObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "TactileObject.h"
#include "Maruyama/Utility/Component/TactileCtrl.h"

namespace basecross {

	void TactileObject::OnCreate() {
		AddComponent<TactileCtrl>();

		auto col = AddComponent<CollisionObb>();
		//col->SetDrawActive(true);
		col->SetAfterCollision(AfterCollision::None);

		transform->SetScale(0.1f, 0.1f, 1.0f);
	}

}

//endbasecross