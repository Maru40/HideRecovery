
/*!
@file SparseGraphBase.h
@brief SparseGraphBase‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// ‘O•ûéŒ¾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;

		//--------------------------------------------------------------------------------------
		///	”Ä—pŒ^ƒOƒ‰ƒt(‘O‰ñì¬‚µ‚½SparseGraph‚Ì‰ü—ÇŒ^A«—ˆ“I‚ÉSparseGraph‚Ííœ—\’è)
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType‚ªNodeBase‚ğŒp³‚µ‚Ä‚¢‚é‚±‚Æ‚ğ•ÛØ‚·‚é
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType‚ªEdgeBase‚ğŒp³‚µ‚Ä‚¢‚é‚±‚Æ‚ğ•ÛØ‚·‚é
			std::nullptr_t
		> = nullptr>
		class SparseGraphBase
		{
		public:

		};

	}
}