
/*!
@file GraphStateNode.cpp
@brief GraphStateNode�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "GraphNodeBase.h"
#include "GraphStateNode.h"

#include "Maruyama/Enemy/StateMachine/StateNodeBase.h"

namespace basecross {

	GraphStateNode::GraphStateNode(
		const std::shared_ptr<I_StateNode>& stateNode
	) :
		GraphStateNode(0, stateNode)
	{}

	GraphStateNode::GraphStateNode(
		const int& index,
		const std::shared_ptr<I_StateNode>& stateNode
	) :
		GraphNodeBase(index),
		m_stateNode(stateNode)
	{}

	void GraphStateNode::OnStart() {
		m_stateNode->OnStart();
	}

	void GraphStateNode::OnUpdate() {
		m_stateNode->OnUpdate();
	}

	void GraphStateNode::OnExit() {
		m_stateNode->OnExit();
	}

}