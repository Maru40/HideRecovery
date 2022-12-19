
/*!
@file AstarEdge.h
@brief AstarEdge�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StateMachine/EdgeBase.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class AstarNode;
		class Debug_DrawController;

		//--------------------------------------------------------------------------------------
		/// AstarNode�̐V�K��
		//--------------------------------------------------------------------------------------
		class AstarEdge : public EdgeBase
		{
		public:
			AstarEdge();

			AstarEdge(
				const std::shared_ptr<AstarNode>& fromNode,
				const std::shared_ptr<AstarNode>& toNode
			);

			std::shared_ptr<AstarNode> GetFromAstarNode();

			std::shared_ptr<AstarNode> GetToAstarNode();
				
			//--------------------------------------------------------------------------------------
			/// �f�o�b�O
			//--------------------------------------------------------------------------------------

			std::shared_ptr<Debug_DrawController> m_debugDrawController;	//�f�o�b�O�\��

			void OnDebugDraw();
		};

	}
}