/*!
@file AstarEdgeDraw.h
@brief AstarEdgeDraw
担当者：丸山 裕喜
*/

#include "../SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "AstarEdgeDraw.h"

#include "Maruyama/Debug/Object/DebugObject.h"
#include "Maruyama/Debug/Object/ArrowObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	エッジデータ
	//--------------------------------------------------------------------------------------

	EdgeData::EdgeData(const std::shared_ptr<AstarEdge>& astarEdge, const std::shared_ptr<GameObject>& drawObject)
		:astarEdge(astarEdge), drawObject(drawObject)
	{}

	//--------------------------------------------------------------------------------------
	///	Astarエッジ表示本体
	//--------------------------------------------------------------------------------------

	AstarEdgeDraw::AstarEdgeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar)
		:Component(objPtr), m_astar(astar)
	{}

	Vec3 AstarEdgeDraw::CalculateCreatePosition(const Vec3& startPosition, const Vec3& endPosition) const {
		auto toVec = endPosition - startPosition;
		auto halfToVec = toVec * 0.5f;
		auto createPosition = startPosition + halfToVec;

		return createPosition;
	}

	Vec3 AstarEdgeDraw::CalculateCreateScale(const Vec3& startPosition, const Vec3& endPosition) const {
		constexpr float SCALE_ADJUST = 0.8f;
		constexpr float SCALE_Y = 0.2f;
		constexpr float SCALE_Z = 1.0f;
		auto toVec = endPosition - startPosition;
		auto scale = Vec3(toVec.length() * SCALE_ADJUST, SCALE_Y, SCALE_Z);

		return scale;
	}

	void AstarEdgeDraw::SettingDrawPositionRotateScale(const std::shared_ptr<GameObject>& object, const Vec3& startPosition, const Vec3& endPosition) {
		auto toVec = endPosition - startPosition;
		auto objectTrans = object->GetComponent<Transform>();

		//ポジションのセット
		objectTrans->SetPosition(CalculateCreatePosition(startPosition, endPosition));

		//スケールのセット
		objectTrans->SetScale(CalculateCreateScale(startPosition, endPosition));

		//向きのセット
		objectTrans->SetForward(toVec.GetNormalized());
		objectTrans->Rotate(Vec3(0.0f, -XM_PIDIV2, 0.0f));
	}

	EdgeData AstarEdgeDraw::CreateEdgeData(const std::shared_ptr<AstarEdge>& edge, const Vec3& startPosition, const Vec3& endPosition) {
		auto toVec = endPosition - startPosition;
		auto createPosition = CalculateCreatePosition(startPosition, endPosition);
			
		auto object = InstantiateArrowObject(createPosition, Quat::Identity());
		SettingDrawPositionRotateScale(object, startPosition, endPosition);

		return EdgeData(edge, object);
	}

	std::shared_ptr<ArrowObject> AstarEdgeDraw::InstantiateArrowObject(const Vec3& position, const Quat& quat) {
		//auto object = ObjectFactory::Create<ArrowObject>(GetStage());
		auto object = GetStage()->AddGameObject<ArrowObject>();
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetPosition(position);
		objectTrans->SetQuaternion(quat);

		return object;
	}

	void AstarEdgeDraw::OnCreate() {
		auto graph = m_astar->GetGraph();
		auto numNode = graph->GetNumNodes();

		DebugObject::AddString(L"ノードの数：", false);
		DebugObject::AddValue(numNode);
		DebugObject::AddString(L"エッジの数：", false);
		DebugObject::AddValue(graph->GetNumEdges());

		for (int i = 0; i < numNode; i++) {
			auto edges = graph->GetEdges(i);
			for (const auto& edge : edges) {
				auto startPosition = graph->GetNode(edge->GetFrom())->GetPosition();
				auto endPosition = graph->GetNode(edge->GetTo())->GetPosition();

				m_edgeDatas.push_back(CreateEdgeData(edge, startPosition, endPosition));
			}
		}
	}

	void AstarEdgeDraw::OnUpdate() {
		if (GetDebugDraw()) {
			UpdatePosition();
		}
	}

	void AstarEdgeDraw::OnDestroy() {
		for (auto& data : m_edgeDatas) {
			if (data.drawObject) {
				GetStage()->RemoveGameObject<GameObject>(data.drawObject.GetShard());
			}
		}
	}

	void AstarEdgeDraw::UpdatePosition() {
		if (!m_astar) {
			return;
		}

		auto graph = m_astar->GetGraph();
		auto numNode = graph->GetNumNodes();

		int index = 0;
		for (int i = 0; i < numNode; i++) {
			auto edges = graph->GetEdges(i);
			for (const auto& edge : edges) {
				auto startPosition = graph->GetNode(edge->GetFrom())->GetPosition();
				auto endPosition = graph->GetNode(edge->GetTo())->GetPosition();

				//生成している表示オブジェクトが足りなくなったら。
				if (m_edgeDatas.size() <= index) {
					m_edgeDatas.push_back(CreateEdgeData(edge, startPosition, endPosition));
					continue;
				}

				const auto& drawObject = m_edgeDatas[index++].drawObject.GetShard();
				drawObject->SetDrawActive(true);
				SettingDrawPositionRotateScale(drawObject, startPosition, endPosition);
			}
		}

		//いらない表示を非表示に
		while (index < m_edgeDatas.size()) {
			m_edgeDatas[index++].drawObject->SetDrawActive(false);
		}

	}

	std::shared_ptr<GameObject> AstarEdgeDraw::FindEdgeDrawObject(const std::shared_ptr<AstarEdge>& edge) {
		for (auto& data : m_edgeDatas) {
			const auto& astarEdge = data.astarEdge;
			if (astarEdge.GetShard() == edge) {
				return data.drawObject.GetShard();
			}
		}

		return nullptr;
	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void AstarEdgeDraw::SetDebugDraw(const bool isDraw) {
		for (auto edge : m_edgeDatas) {
			auto drawObject = edge.drawObject;
			drawObject->SetDrawActive(isDraw);
		}
	}

	bool AstarEdgeDraw::GetDebugDraw() const {
		return m_edgeDatas.empty() ? false : m_edgeDatas[0].drawObject->GetDrawActive();
	}

}

//endbasecross