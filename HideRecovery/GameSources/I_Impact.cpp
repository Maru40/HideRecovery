
/*!
@file I_Impact.cpp
@brief I_Impactのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "EyeSearchRange.h"
#include "NavGraphNode.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------

		ImpactData::ImpactData():
			dangerValue(1.0f), 
			occupancyValue(0.5f),
			occupancyType(OccupancyType::Empty)
		{}

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