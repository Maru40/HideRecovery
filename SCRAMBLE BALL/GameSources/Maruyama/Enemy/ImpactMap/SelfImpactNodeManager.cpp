
/*!
@file SelfImpactNodeManager.h
@brief SelfImpactNodeManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SelfImpactNodeManager.h"

#include "Maruyama/Interface/I_Impact.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		SelfImpactNodeManager_Parametor::SelfImpactNodeManager_Parametor() :
			range(5.0f),
			muchFarRange(15.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �e���}�b�v�̒��Ŏ������g�̃m�[�h���Ǘ�����R���|�[�l���g
		//--------------------------------------------------------------------------------------

		SelfImpactNodeManager::SelfImpactNodeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<I_Impacter>& impacter):
			Component(objPtr), m_impacter(impacter), m_param(Parametor())
		{}

		void SelfImpactNodeManager::OnUpdate() {
			if (!m_impacter.lock()) {	//�e����^����҂����݂��Ȃ��Ȃ�
				return;
			}

			SelfNodeUpdate();
		}

		void SelfImpactNodeManager::SelfNodeUpdate() {
			auto impacter = m_impacter.lock();
			auto impactMap = impacter->GetImpactMap();
			std::shared_ptr<NavGraphNode> selfNode = impacter->GetSelfNode();
			if (!selfNode) {	//�m�[�h���ݒ肳��Ă��Ȃ��Ȃ�
				auto node = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), transform->GetPosition());
				impacter->SetSelfNode(node);
				return;
			}

			Vec3 toSelfNode = selfNode->GetPosition() - transform->GetPosition();

			//�������g�̃m�[�h���痣�ꂽ��
			if (m_param.range < toSelfNode.length()) {
				SettingSelfNode();	//�m�[�h�̐؂�ւ�
			}
		}

		void SelfImpactNodeManager::SettingSelfNode() {
			auto impacter = m_impacter.lock();
			auto impactMap = impacter->GetImpactMap();
			auto selfNode = impacter->GetSelfNode();
			
			auto astar = impactMap->GetGraphAstar();
			
			auto targetNode = UtilityAstar::CalculateTargetDirectNode(*astar.get(), selfNode, transform->GetPosition());

			impacter->SetSelfNode(targetNode);
		}
	}
}