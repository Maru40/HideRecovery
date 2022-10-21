
/*!
@file NavGraphNode.cpp
@brief NavGraphNodeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "NavGraphNode.h"

#include "Maruyama/Utility/MaruAction.h"
#include "Maruyama/Utility/ReactiveProperty.h"

namespace basecross {

	NavGraphNode::NavGraphNode()
		: NavGraphNode(0)
	{ }

	NavGraphNode::NavGraphNode(const int& index)
		: NavGraphNode(index, Vec3(0.0f))
	{ }

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position)
		: NavGraphNode(index, position, ImpactData())
	{ }

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position, const ImpactData& impactData)
		: NavGraphNode(index, position, impactData, nullptr)
	{}

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent)
		: GraphNodeBase(index), m_position(position), m_impactData(new ReactiveProperty<ImpactData>(impactData)), m_parent(parent) 
	{}

	//コピーコンストラクタ
	NavGraphNode::NavGraphNode(const NavGraphNode& node) :
		NavGraphNode(node.GetIndex(), node.GetPosition(), node.GetImpactData(), node.GetParent())
	{}

	NavGraphNode::~NavGraphNode() = default;

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	Vec3 NavGraphNode::GetPosition() const {
		//親の設定があるなら、親の場所の相対位置を返す。
		if (IsParent()) {
			auto parentTrans = GetParent()->GetComponent<Transform>();
			return m_position + parentTrans->GetWorldPosition();
		}

		return m_position;
	}

	void NavGraphNode::SetImpactData(const ImpactData& data) noexcept { 
		m_impactData->SetValue(data); 
	}

	maru::ImpactData NavGraphNode::GetImpactData() const noexcept { 
		return m_impactData->GetValue(); 
	}

	void NavGraphNode::AddSubscribeImpactData(const std::function<bool()>& whereAction, const std::function<void()>& action) {
		m_impactData->AddSubscribe(whereAction, action);
	}

	void NavGraphNode::SetAreaIndex(const int index) noexcept {
		m_impactData->GetRefValue().areaIndex = index;
	}

	int NavGraphNode::GetAreaIndex() const noexcept {
		return m_impactData->GetValue().areaIndex;
	}

	//--------------------------------------------------------------------------------------
	///	オペレータ
	//--------------------------------------------------------------------------------------

	NavGraphNode& NavGraphNode::operator= (const NavGraphNode& other) {
		m_position = other.m_position;
		m_impactData.reset(new ReactiveProperty<ImpactData>(other.GetImpactData()));
		m_parent = other.m_parent;

		return *this;
	}
}

//endbasecross