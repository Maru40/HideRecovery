
/*!
@file FieldImpactMap.cpp
@brief FieldImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "FieldImpactMap.h"

#include "Maruyama/Load/StageMapCSV.h"
#include "ImpactMap.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "Maruyama/Utility/MaruAction.h"
#include "Maruyama/Utility/Utility.h"

#include "OccupancyManager.h"

namespace basecross {

	namespace maru {

		std::weak_ptr<FieldImpactMap> SingletonComponent<FieldImpactMap>::sm_instance;

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v�̃p�����[�^
		//--------------------------------------------------------------------------------------

		FieldImpactMap_Parametor::FieldImpactMap_Parametor():
			intervalRange(10.0f),
			createHeightOffset(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v�{��
		//--------------------------------------------------------------------------------------

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors
		):
			FieldImpactMap(objPtr, floors, Parametor())
		{}

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors, 
			const Parametor& parametor
		):
			SingletonComponent(objPtr), 
			m_floors(maru::Utility::ConvertArraySharedToWeak(floors)), 
			m_param(parametor),
			m_impactMap(new ImpactMap(GetGameObject()->GetStage())),
			m_occupancyManager(new OccupancyManager(m_impactMap))
		{}

		void FieldImpactMap::OnLateStart() {
			//m_impactMap = std::make_shared<ImpactMap>(GetStage()); //ImpactMap�̐���

			m_impactMap->ClearNodes();  //�m�[�h�̃N���A
			m_impactMap->ClearEdges();  //�G�b�W�̃N���A
			const float IntervalRange = m_param.intervalRange; //�e���}�b�v�̊e�m�[�h�̋���
			SettingImpactMap(m_floors, IntervalRange);

			m_impactMap->CreateDebugDraw(false); //�f�o�b�O�f�[�^�̐���
		}

		void FieldImpactMap::OnUpdate() {
			OccupancyUpdate();

			//�f�o�b�O
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsYDown(); });
			}
		}

		void FieldImpactMap::OccupancyUpdate() {
			if (m_occupancyManager) {
				m_occupancyManager->OnUpdate();
			}
		}

		std::vector<Vec3> FieldImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			return m_impactMap->GetRoutePositons(selfPosition, targetPosition);
		}

		std::shared_ptr<ImpactMap> FieldImpactMap::GetImpactMap() const {
			return m_impactMap;
		}

		void FieldImpactMap::AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node) {
			if (m_occupancyManager) {
				m_occupancyManager->AddOccupancyUpdateData(node);
			}
		}

	}
}