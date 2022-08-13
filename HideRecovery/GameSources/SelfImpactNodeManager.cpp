
/*!
@file SelfImpactNodeManager.h
@brief SelfImpactNodeManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SelfImpactNodeManager.h"

#include "I_Impact.h"
#include "ImpactMap.h"
#include "GraphAstar.h"
#include "SparseGraph.h"
#include "AstarEdge.h"
#include "NavGraphNode.h"

#include "MaruUtility.h"
#include "UtilityAstar.h"

namespace basecross {

	namespace maru {

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
			std::shared_ptr<NavGraphNode> selfNode = impacter->GetSelfNode();

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