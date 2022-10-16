
/*!
@file SoleImpactMap.cpp
@brief SoleImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SoleImpactMap.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"

#include "Maruyama/Interface/I_Impact.h"
#include "Maruyama/Enemy/ImpactMap/ImpactManager.h"

namespace basecross {

	namespace maru {

		SoleImpactMap::SoleImpactMap(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr)
		{}

		SoleImpactMap::SoleImpactMap(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
			Component(objPtr), m_param(parametor)
		{}

		void SoleImpactMap::OnCreate() {
			CreateImpactMap();
		}

		void SoleImpactMap::OnUpdate() {
			//�f�o�b�O�p
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsXDown(); });
			}
		}

		void SoleImpactMap::CreateImpactMap() {
			auto impactMap = std::make_shared<ImpactMap>(GetStage(), m_param.factoryParam);
			impactMap->GetGraphAstar()->SetNodesParent(GetGameObject());
			impactMap->CreateDebugDraw(false);  //�f�o�b�O�\���𐶐�

			m_impactMap = impactMap;
		}

		void SoleImpactMap::CalculateImpact(const std::shared_ptr<I_Impacter>& impacter) {
			//�e���}�b�v�Ǘ��N���X�̐���
			auto impactManager = std::make_shared<ImpactManager>();

			//�e���}�b�v�Ǘ��N���X�Ɍv�Z������B
			impactManager->CalculateAllImpact(m_impactMap, impacter);
		}


		//�@�m�[�h���v���C���[�̑��Έʒu�ɍ��킹�Ĉړ�������B
		//�A
		
	}
}