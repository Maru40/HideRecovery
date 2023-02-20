
/*!
@file AstarEdge.cpp
@brief AstarEdge�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Ticket_AstarRoute.h"

#include "Maruyama/Enemy/Astar/OpenDataHandler.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {
	namespace Ticket {

		void AstarRoute::Start_RouteSearch(
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		) {
			SetIsValid(false);	//�����J�n�������߁A������ԂɕύX(�����I�ɂ͕ʂȏꏊ�ŕύX)

			maru::Utility::QueueClear(m_route);	//���݂̃��[�g�̃N���A
			
			auto openHandler = OpenDataHandler();	//�I�[�v���f�[�^�Ǘ��N���X
			//openHandler.StartSearchAstar(startNode, targetNode, graph);
		}

	}
}