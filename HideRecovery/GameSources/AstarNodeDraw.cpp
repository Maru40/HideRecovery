
/*!
@file AstarNodeDraw.cpp
@brief AstarNodeDraw�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SparseGraph.h"
#include "AstarEdge.h"
#include "NavGraphNode.h"
#include "GraphAstar.h"

#include "NumbersCtrl.h"
#include "NumbersObject.h"
#include "AstarNodeDraw.h"

namespace basecross {

	AstarNodeDraw::AstarNodeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar)
		:Component(objPtr), m_astar(astar)
	{}

	void AstarNodeDraw::OnCreate() {
		CreateNodeDraw();
	}

	void AstarNodeDraw::CreateNodeDraw() {
		auto graph = m_astar->GetGraph();
		auto nodes = graph->GetNodes();

		for (const auto& node : nodes) {
			//�m�[�h�̕\��
			auto numberObject = ObjectFactory::Create<NumbersObject>(GetStage());
			//auto numberObject = GetStage()->AddGameObject<NumbersObject>();
			numberObject->GetComponent<Transform>()->SetPosition(node->GetPosition());
			numberObject->GetComponent<NumbersCtrl>()->SetValue(node->GetIndex());
			//auto index = numberObject->GetComponent<NumbersCtrl>()->GetValue();

			m_nodes.push_back(numberObject);
			//SetColor(node->GetIndex(), Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	void AstarNodeDraw::UpdateNodesPosition() {
		if (!m_astar) {
			return;
		}

		//NumberObject�̃��[�v
		for (auto& number : m_nodes) {
			//Graph�m�[�h�̃��[�v
			for (const auto& graphNode : m_astar->GetGraph()->GetNodes()) {
				//�����C���f�b�N�X�Ȃ�
				if (number->GetComponent<NumbersCtrl>()->GetValue() == graphNode->GetIndex()) {
					number->GetComponent<Transform>()->SetPosition(graphNode->GetPosition());
					break;
				}
			}
		}
	}

	void AstarNodeDraw::OnUpdate() {


		for (const auto& node : m_nodes) {
			node->OnUpdate();
			node->ComponentUpdate();
		}

		//Draw��Active�̎��̂�
		if (IsDrawActive()) {
			UpdateNodesPosition();
		}
	}

	void AstarNodeDraw::OnDraw() {
		if (IsDrawActive() == false) {
			return;
		}

		Component::OnDraw();

		for (const auto& node : m_nodes) {
			node->OnDraw();
		}
	}

	//--------------------------------------------------------------------------------------
	///	�A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void AstarNodeDraw::SetDebugDraw(const bool isDraw) {
		for (auto& node : m_nodes) {
			node->SetDrawActive(isDraw);
		}
	}
	bool AstarNodeDraw::GetDebugDraw() const {
		return m_nodes.empty() ? false : m_nodes[0]->GetDrawActive();
	}

	void AstarNodeDraw::SetColor(const int& nodeIndex, const Col4& color) {
		for (auto& node : m_nodes) {
			auto number = node->GetComponent<NumbersCtrl>(false);
			auto index = number->GetValue();
			if (number->GetValue() == nodeIndex) {
				node->GetComponent<NumbersCtrl>()->SetColor(color);
				break;
			}
		}
	}

}

//endbasecross