
/*!
@file GeneratorBase.cpp
@brief GeneratorBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "GeneratorBase.h"

namespace basecross {

	GeneratorBase::GeneratorBase(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	int GeneratorBase::GetNumObject() const {
		int count = 0;
		for (auto& object : m_objects) {
			if (!object) {
				continue;
			}

			//SlimeでDyning、又は、Death状態ならカウントしない。
			//if (auto slimeStator = object->GetComponent<Enemy::I_Stator_EnumType<Enemy::SlimeStateType>>(false)) {
			//	if (slimeStator->IsCurrentState(Enemy::SlimeStateType::Dyning) || 
			//		slimeStator->IsCurrentState(Enemy::SlimeStateType::Death) || 
			//		slimeStator->IsCurrentState(Enemy::SlimeStateType::Pressed)
			//	) {
			//		continue;
			//	}
			//}
			
			count++;
		}

		return count;
	}

	std::vector<ex_weak_ptr<GameObject>> GeneratorBase::GetObjects() const {
		return m_objects;
	}
}

//endbasecross