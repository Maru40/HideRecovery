/*!
@file SettingStartTarget.cpp
@brief SettingStartTarget�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/Component/Targeted.h"

#include "SettingStartTarget.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//void SettingStartTarget::OnStart() {
				//	for (auto& target : m_targets) {
				//		auto targeted = target.lock()->GetComponent<Targeted>(false);
				//		if (!targeted) {	//�R���|�[�l���g�����݂��Ȃ��B
				//			continue;
				//		}

				//		if (!targeted->CanTarget()) {	//�^�[�Q�b�g�w��ł��Ȃ�
				//			continue;
				//		}


				//	}
				//}

			}
		}
	}
}