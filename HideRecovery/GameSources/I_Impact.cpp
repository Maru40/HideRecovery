
/*!
@file I_Impact.cpp
@brief I_ImpactÌNXÀÌ
SFÛRTì
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "EyeSearchRange.h"
#include "NavGraphNode.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	e¿f[^
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			dangerValue(1.0f), 
			occupancyValue(0.5f),
			occupancyType(OccupancyType::Empty)
		{}

		ImpactData::~ImpactData() {};

		//--------------------------------------------------------------------------------------
		///	e¿ð^¦éÒÌf[^
		//--------------------------------------------------------------------------------------

		ImpacterData::ImpacterData():
			value(0.0f),
			circleRange(0.0f),
			type(ImpacterType(0))
		{}

		//--------------------------------------------------------------------------------------
		///	e¿ð^¦éÒÌ{Ì
		//--------------------------------------------------------------------------------------

		void I_Impacter::SetSelfNode(const std::shared_ptr<NavGraphNode>& node) {
			m_impacterData.selfNode = node;
		}

		std::shared_ptr<NavGraphNode> I_Impacter::GetSelfNode() const {
			return m_impacterData.selfNode.lock();
		}
	}
}