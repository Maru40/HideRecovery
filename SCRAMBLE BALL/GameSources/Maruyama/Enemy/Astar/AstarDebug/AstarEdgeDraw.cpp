/*!
@file AstarEdgeDraw.h
@brief AstarEdgeDraw
�S���ҁF�ێR �T��
*/

#include "../SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "AstarEdgeDraw.h"

#include "Maruyama/DebugClass/Object/DebugObject.h"
#include "Maruyama/DebugClass/Object/ArrowObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�G�b�W�f�[�^
	//--------------------------------------------------------------------------------------

	EdgeData::EdgeData(const std::shared_ptr<AstarEdge>& astarEdge, const std::shared_ptr<GameObject>& drawObject)
		:astarEdge(astarEdge), drawObject(drawObject)
	{}

	//--------------------------------------------------------------------------------------
	///	Astar�G�b�W�\���{��
	//--------------------------------------------------------------------------------------

	AstarEdgeDraw::AstarEdgeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<const GraphAstar::GraphType>& astar)
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
		constexpr float SCALE_Y = 0.25f;
		constexpr float SCALE_Z = 1.0f;
		auto toVec = endPosition - startPosition;
		auto scale = Vec3(toVec.length() * SCALE_ADJUST, SCALE_Y, SCALE_Z);

		return scale;
	}

	void AstarEdgeDraw::SettingDrawPositionRotateScale(const std::shared_ptr<GameObject>& object, const Vec3& startPosition, const Vec3& endPosition) {
		auto toVec = endPosition - startPosition;
		auto objectTrans = object->GetComponent<Transform>();

		//�|�W�V�����̃Z�b�g
		objectTrans->SetPosition(CalculateCreatePosition(startPosition, endPosition));

		//�X�P�[���̃Z�b�g
		objectTrans->SetScale(CalculateCreateScale(startPosition, endPosition));

		//�����̃Z�b�g
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
		auto graph = m_astar.lock();
		auto numNode = graph->GetNumNodes();

		DebugObject::AddString(L"�m�[�h�̐��F", false);
		DebugObject::AddValue(numNode);
		DebugObject::AddString(L"�G�b�W�̐��F", false);
		DebugObject::AddValue(graph->GetNumEdges());

		for (int i = 0; i < numNode; i++) {
			auto edges = graph->GetEdges(i);
			for (const auto& edge : edges) {
				auto fromIndex = edge->GetFrom();
				auto toIndex = edge->GetTo();
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
		if (!m_astar.lock()) {
			return;
		}

		auto graph = m_astar.lock();
		auto numNode = graph->GetNumNodes();

		int index = 0;
		for (int i = 0; i < numNode; i++) {
			auto edges = graph->GetEdges(i);
			for (const auto& edge : edges) {
				auto startPosition = graph->GetNode(edge->GetFrom())->GetPosition();
				auto endPosition = graph->GetNode(edge->GetTo())->GetPosition();

				//�������Ă���\���I�u�W�F�N�g������Ȃ��Ȃ�����B
				if (m_edgeDatas.size() <= index) {
					m_edgeDatas.push_back(CreateEdgeData(edge, startPosition, endPosition));
					continue;
				}

				const auto& drawObject = m_edgeDatas[index++].drawObject.GetShard();
				drawObject->SetDrawActive(true);
				SettingDrawPositionRotateScale(drawObject, startPosition, endPosition);
			}
		}

		//����Ȃ��\�����\����
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
	///	�A�N�Z�b�T
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