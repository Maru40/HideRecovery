/*!
@file AuraObjectBase.cpp
@brief AuraObjectBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AuraObjectBase.h"

#include "Maruyama/Effect/Component/AuraBase.h"

namespace basecross {
	namespace Effect {

		AuraObjectBase::AuraObjectBase(const std::shared_ptr<Stage>& stage, const Aura_Parametor& parametor):
			GameObject(stage), m_auraParamPtr(new Aura_Parametor(parametor))
		{}

		void AuraObjectBase::OnCreate() {
			AddComponent<AuraBase>(*m_auraParamPtr.get());
		}

	}
}