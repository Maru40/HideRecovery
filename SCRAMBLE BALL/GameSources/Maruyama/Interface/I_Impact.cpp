
/*!
@file I_Impact.cpp
@brief I_Impact�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^
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
		///	�e����^����҂̃f�[�^
		//--------------------------------------------------------------------------------------

		ImpacterData::ImpacterData():
			value(0.0f),
			circleRange(0.0f),
			type(ImpacterType(0))
		{}

		//--------------------------------------------------------------------------------------
		///	�e����^����҂̖{��
		//--------------------------------------------------------------------------------------

		void I_Impacter::SetSelfNode(const std::shared_ptr<NavGraphNode>& node) {
			m_impacterData.selfNode = node;
		}

		std::shared_ptr<NavGraphNode> I_Impacter::GetSelfNode() const {
			return m_impacterData.selfNode.lock();
		}
	}
}