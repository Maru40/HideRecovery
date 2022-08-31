
/*!
@file AnimationNodeBase.h
@brief AnimationNodeBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		template<class NodeType>
		class AnimationNodeBase;

		template<class NodeType>
		class AnimationNodeBase : public StateNodeBase<NodeType>
		{
		public:

		private:

		public:
			AnimationNodeBase(const std::shared_ptr<NodeType>& owner)
				:StateNodeBase(owner)
			{}

		private:

		public:

		};

	}

}