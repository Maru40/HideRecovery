
/*!
@file AstarNodeDraw.cpp
@brief AstarNodeDrawクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"

#include "Maruyama/UI/3D/Component/NumbersCtrl.h"
#include "Maruyama/UI/3D/Object/NumbersObject.h"
#include "AstarNodeDraw.h"

#include "ImpactDataDraw.h"

#include "Maruyama/Debug/Object/DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	デバッグ表示用のデータ
	//--------------------------------------------------------------------------------------

	DrawNodeData::DrawNodeData(const std::shared_ptr<NavGraphNode>& node) :
		node(node), 
		nodeNumber(nullptr), 
		impactData(nullptr)
	{}

	void DrawNodeData::OnUpdate() {
		if (nodeNumber) {
			nodeNumber->OnUpdate();
			nodeNumber->ComponentUpdate();
		}
		
		if (impactData) {
			impactData->OnUpdate();
			impactData->ComponentUpdate();
		}
	}

	void DrawNodeData::OnDraw() {
		if (nodeNumber) {
			nodeNumber->OnDraw();
			nodeNumber->ComponentDraw();
		}

		if (impactData) {
			impactData->OnDraw();
			impactData->ComponentDraw();
		}
	}

	void DrawNodeData::SetDrawActive(const bool isActive) {
		if (nodeNumber) {
			nodeNumber->SetDrawActive(isActive);
		}

		if (impactData) {
			impactData->SetDrawActive(isActive);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Astarノード表示本体
	//--------------------------------------------------------------------------------------

	AstarNodeDraw::AstarNodeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar)
		:Component(objPtr), m_astar(astar)
	{}

	void AstarNodeDraw::OnCreate() {
		CreateDrawDatas();
	}

	void AstarNodeDraw::OnUpdate() {
		//DrawがActiveの時のみ
		if (IsDrawActive()) {
			UpdateNodes();

			for (auto& data : m_drawDatas) {
				data.OnUpdate();
			}
		}
	}

	void AstarNodeDraw::OnDraw() {
		if (IsDrawActive()) {
			Component::OnDraw();

			for (auto& data : m_drawDatas) {
				data.OnDraw();
			}
		}
	}

	void AstarNodeDraw::OnDestroy() {
		m_drawDatas.clear();
	}

	void AstarNodeDraw::CreateDrawDatas() {
		auto graph = m_astar->GetGraph();
		auto nodes = graph->GetNodes();

		for (const auto& node : nodes) {
			auto data = DrawNodeData(nullptr);

			//データの中身を格納
			data.nodeNumber = CreateNodeNumber(node);
			data.impactData = CreateImpactDataDraw(node);

			m_drawDatas.push_back(data);	//配列に追加
		}
	}

	std::shared_ptr<NumbersObject> AstarNodeDraw::CreateNodeNumber(const std::shared_ptr<NavGraphNode>& node) {
		//ノードの番号表示
		auto numberObject = ObjectFactory::Create<NumbersObject>(GetStage());
		numberObject->GetComponent<Transform>()->SetPosition(node->GetPosition());
		auto numberCtrl = numberObject->GetComponent<NumbersCtrl>();
		numberCtrl->SetValue(static_cast<float>(node->GetIndex()));

		//ナンバーの色を占有マップ値に合わせて変更(デバッグ)
		auto value = node->GetImpactData().occupancyValue;
		numberCtrl->SetColor(Col4(1.0f, value, value, 1.0f));

		return numberObject;
	}

	std::shared_ptr<ImpactDataDrawObject> AstarNodeDraw::CreateImpactDataDraw(const std::shared_ptr<NavGraphNode>& node) {
		auto impactDataDraw = ObjectFactory::Create<ImpactDataDrawObject>(GetStage(), node);
		auto drawTrans = impactDataDraw->GetComponent<Transform>();		//transformの取得
		drawTrans->SetPosition(node->GetPosition());

		return impactDataDraw;
	}

	void AstarNodeDraw::UpdateNodes() {
		if (!m_astar) {
			return;
		}

		//NumberObjectのループ
		for (auto& data : m_drawDatas) {
			const auto& number = data.nodeNumber;

			//Graphノードのループ
			for (const auto& node : m_astar->GetGraph()->GetNodes()) {
				//同じインデックスなら
				int index = static_cast<int>(number->GetComponent<NumbersCtrl>()->GetValue());

				if (index == node->GetIndex()) {
					//位置を設定
					number->GetComponent<Transform>()->SetPosition(node->GetPosition());
					data.impactData->GetComponent<Transform>()->SetPosition(node->GetPosition());
					
					break;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void AstarNodeDraw::SetDebugDraw(const bool isDraw) {
		for (auto& data : m_drawDatas) {
			data.SetDrawActive(isDraw);
		}
	}
	bool AstarNodeDraw::GetDebugDraw() const {
		return m_drawDatas.empty() ? false : m_drawDatas[0].nodeNumber->GetDrawActive();
	}

	void AstarNodeDraw::SetColor(const int& nodeIndex, const Col4& color) {
		for (auto& data : m_drawDatas) {
			auto number = data.nodeNumber->GetComponent<NumbersCtrl>(false);
			auto index = number->GetValue();
			if (number->GetValue() == nodeIndex) {
				number->SetColor(color);
				break;
			}
		}
	}

}

//endbasecross