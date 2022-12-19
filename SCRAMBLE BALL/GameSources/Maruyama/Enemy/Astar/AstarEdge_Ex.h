
/*!
@file AstarEdge.h
@brief AstarEdgeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StateMachine/EdgeBase.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class AstarNode;
		class Debug_DrawController;

		//--------------------------------------------------------------------------------------
		/// AstarNodeの新規版
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
			/// デバッグ
			//--------------------------------------------------------------------------------------

			std::shared_ptr<Debug_DrawController> m_debugDrawController;	//デバッグ表示

			void OnDebugDraw();
		};

	}
}