
/*!
@file EdgeBase.h
@brief EdgeBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		class NodeBase;

		//--------------------------------------------------------------------------------------
		/// エッジのインターフェース
		//--------------------------------------------------------------------------------------
		class I_Edge {
			virtual void SetFromIndex(const int index) noexcept = 0;
			virtual int GetFromIndex() const noexcept = 0;

			virtual void SetToIndex(const int index) noexcept = 0;
			virtual int GetToIndex() const noexcept = 0;
		};

		//--------------------------------------------------------------------------------------
		/// エッジの基底クラス(最低限の機能を持たせる。)i
		//--------------------------------------------------------------------------------------
		class EdgeBase : public I_Edge
		{
			std::weak_ptr<NodeBase> m_fromNode;
			std::weak_ptr<NodeBase> m_toNode;

		public:
			EdgeBase();
			EdgeBase(const std::shared_ptr<NodeBase>& fromNode, const std::shared_ptr<NodeBase>& toNode);

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 手前のノードの設定
			/// </summary>
			/// <param name="node">手前のノード</param>
			void SetFromNode(const std::shared_ptr<NodeBase>& node);

			std::shared_ptr<NodeBase> GetFromNode() const;

			void SetFromIndex(const int index) noexcept override;

			int GetFromIndex() const noexcept override;

			void SetToNode(const std::shared_ptr<NodeBase>& node);

			std::shared_ptr<NodeBase> GetToNode() const;

			void SetToIndex(const int index) noexcept override;

			int GetToIndex() const noexcept override;
		};

	}
}