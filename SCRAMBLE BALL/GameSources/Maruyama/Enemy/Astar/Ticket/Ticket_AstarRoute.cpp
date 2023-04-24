
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

#include "Maruyama/Enemy/Astar/OpenDataCalculater.h"

#include "Maruyama/Utility/Utility.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace Ticket {

		AstarRoute::AstarRoute():
			m_isValid(false)
		{}

		void AstarRoute::Start_RouteSearch(
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		) {
			SetIsValid(false);								//�����J�n�������߁A������ԂɕύX(�����I�ɂ͕ʂȏꏊ�ŕύX)

			maru::Utility::StackClear(m_route);				//���݂̃��[�g�̃N���A
			
			auto calculater = maru::OpenDataCalculater();	//�I�[�v���f�[�^�Ǘ��N���X
			auto isSuccess = calculater.StartSearchAstar(startNode, targetNode, graph);	//�v�Z�J�n

			if (!isSuccess) {
				Debug::GetInstance()->Log(L"Ticket::AstarRoute::Start_RouteSearch() : ���[�g�����Ɏ��s���܂����B");
			}

			m_route = calculater.GetRoute();	//���[�g�̎擾
			SetIsValid(true);					//�L����ԂɕύX
		}

		AstarRoute::RouteStack AstarRoute::GetRoute() const {
			return m_route;
		}

	}
}