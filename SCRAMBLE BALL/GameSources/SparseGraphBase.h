
/*!
@file SparseGraphBase.h
@brief SparseGraphBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;

		//--------------------------------------------------------------------------------------
		///	�ėp�^�O���t(�O��쐬����SparseGraph�̉��ǌ^�A�����I��SparseGraph�͍폜�\��)
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		> = nullptr>
		class SparseGraphBase
		{
		public:

		};

	}
}