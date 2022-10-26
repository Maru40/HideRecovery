
/*!
@file ImpactMap.cpp
@brief ImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SelfAstarNodeController.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/AstarGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	SelfAstarNodeController::SelfAstarNodeController(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), m_isNodeInitialize(false)
	{}

	void SelfAstarNodeController::OnCreate() {

	}

	void SelfAstarNodeController::OnLateStart() {
		InitializeNode();
	}

	void SelfAstarNodeController::OnUpdate() {
		UpdateProccess();
	}

	void SelfAstarNodeController::UpdateProccess() {
		if (IsInitializeNode()) {
			return;
		}

		if (!HasNode() || IsFarNode()) {
			m_isNodeInitialize = true;
			std::thread initializeNode([&]() { InitializeNode(); });
			initializeNode.detach();
			return;
		}

		UpdateNode();

		//if (HasNode()) {
		//	wstring str = L"";
		//	str += L"NodeIndex: ";
		//	str += std::to_wstring(GetNode()->GetIndex());
		//	Debug::GetInstance()->Log(str);
		//}
		//else {
		//	Debug::GetInstance()->Log(L"ノードがありません。");
		//}
	}

	void SelfAstarNodeController::UpdateNode() {
		if (IsUpdateNode()) {
			return;
		}

		auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();
		auto astar = impactMap->GetGraphAstar();
		auto graph = astar->GetGraph();

		//ソート用のデータ構造体
		struct Data {
			std::weak_ptr<NavGraphNode> node;
			float range;

			Data(const std::shared_ptr<NavGraphNode>& node, const float range):
				node(node), range(range)
			{}
		};

		std::vector<Data> datas;	//ソート用の配列
		//現在のノードを入れる。
		float range = (GetNode()->GetPosition() - transform->GetPosition()).length();
		datas.push_back(Data(GetNode(), range));

		//距離を計ってソート用の配列に代入する。
		for (auto& edge : graph->GetEdges(m_node.lock()->GetIndex())) {
			auto node = graph->GetNode(edge->GetTo());
			float toNodeRange = (node->GetPosition() - transform->GetPosition()).length();

			datas.push_back(Data(node, toNodeRange));
		}

		//ソート条件
		auto sortFunction = [&](const Data& left, const Data& right) {
			return left.range < right.range;
		};
		std::sort(datas.begin(), datas.end(), sortFunction);	//ソート

		m_node = !datas.empty() ? datas[0].node.lock() : nullptr;	//配列が存在するなら新規ノードに変更
	}

	bool SelfAstarNodeController::IsUpdateNode() {
		if (m_node.expired()) {	//ノードが存在しないならアップデートが必要
			return true;
		}

		auto fieldImpactMap = maru::FieldImpactMap::GetInstance();
		float intervalRnage = fieldImpactMap->GetIntervalRange();

		auto toNodeRange = (m_node.lock()->GetPosition() - transform->GetPosition()).length();

		return intervalRnage < toNodeRange;	//距離が遠くなったら更新が必要。
	}

	void SelfAstarNodeController::InitializeNode() {
		auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();
		auto astar = impactMap->GetGraphAstar();
		auto graph = astar->GetGraph();

		auto areaIndex = astar->SearchNearAreaIndex(transform->GetPosition());
		auto node = UtilityAstar::SearchNearNode(astar->GetGraph(areaIndex), transform->GetPosition());
		SetNode(node);

		m_isNodeInitialize = false;
	}

	bool SelfAstarNodeController::IsFarNode() {
		if (!HasNode()) {
			return false;
		}

		auto toNode = GetNode()->GetPosition() - transform->GetPosition();
		float range = toNode.length();
		float intervalRange = maru::FieldImpactMap::GetInstance()->GetIntervalRange();
		constexpr float FarCost = 1.0f;
		float farRange = intervalRange + FarCost;

		return farRange < range;
	}


	void SelfAstarNodeController::SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept {
		m_node = node;
	}

	std::shared_ptr<NavGraphNode> SelfAstarNodeController::GetNode() const noexcept {
		return m_node.lock();
	}

	bool SelfAstarNodeController::HasNode() const noexcept {
		return !m_node.expired();
	}

}