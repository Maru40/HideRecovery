
/*!
@file I_Impact.cpp
@brief I_Impactのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			ImpactData(1)
		{}

		ImpactData::ImpactData(const int areaIndex) :
			dangerValue(1.0f),
			occupancyValue(0.5f),
			occupancyType(OccupancyType::Empty),
			occupacyRecoverySpeed(30.0f),
			areaIndex(areaIndex)
		{}

		ImpactData::~ImpactData() {};

		//--------------------------------------------------------------------------------------
		///	影響を与える者のデータ
		//--------------------------------------------------------------------------------------

		ImpacterData::ImpacterData():
			value(0.0f),
			circleRange(0.0f),
			type(ImpacterType(0))
		{}

		//--------------------------------------------------------------------------------------
		///	影響を与える者の本体
		//--------------------------------------------------------------------------------------

		void I_Impacter::SetSelfNode(const std::shared_ptr<NavGraphNode>& node) {
			m_impacterData.selfNode = node;
		}

		std::shared_ptr<NavGraphNode> I_Impacter::GetSelfNode() const {
			return m_impacterData.selfNode.lock();
		}
	}
}