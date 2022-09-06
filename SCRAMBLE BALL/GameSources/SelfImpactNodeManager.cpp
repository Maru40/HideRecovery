
/*!
@file SelfImpactNodeManager.h
@brief SelfImpactNodeManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SelfImpactNodeManager.h"

#include "Maruyama/Interface/I_Impact.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		SelfImpactNodeManager_Parametor::SelfImpactNodeManager_Parametor() :
			range(5.0f),
			muchFarRange(15.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// 影響マップの中で自分自身のノードを管理するコンポーネント
		//--------------------------------------------------------------------------------------

		SelfImpactNodeManager::SelfImpactNodeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<I_Impacter>& impacter):
			Component(objPtr), m_impacter(impacter), m_param(Parametor())
		{}

		void SelfImpactNodeManager::OnUpdate() {
			if (!m_impacter.lock()) {	//影響を与える者が存在しないなら
				return;
			}

			SelfNodeUpdate();
		}

		void SelfImpactNodeManager::SelfNodeUpdate() {
			auto impacter = m_impacter.lock();
			auto impactMap = impacter->GetImpactMap();
			std::shared_ptr<NavGraphNode> selfNode = impacter->GetSelfNode();
			if (!selfNode) {	//ノードが設定されていないなら
				auto node = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), transform->GetPosition());
				impacter->SetSelfNode(node);
				return;
			}

			Vec3 toSelfNode = selfNode->GetPosition() - transform->GetPosition();

			//自分自身のノードから離れたら
			if (m_param.range < toSelfNode.length()) {
				SettingSelfNode();	//ノードの切り替え
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