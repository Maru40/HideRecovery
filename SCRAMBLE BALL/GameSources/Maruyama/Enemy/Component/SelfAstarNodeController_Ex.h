
/*!
@file SelfAstarNodeController_Ex.h
@brief SelfAstarNodeController_Exなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class AstarNode;
	}

	//--------------------------------------------------------------------------------------
	///	自分自身のAstarNodeを検索依頼を出すインターフェース
	//--------------------------------------------------------------------------------------
	class I_SelfAstarNodeRequester : public I_ThreadRequester {
	public:
		virtual ~I_SelfAstarNodeRequester() = default;

		/// <summary>
		/// 自分自身のノードを検索する。
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<maru::AstarNode> Search_SelfAstarNode() = 0;
	};

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// 自分自身が所属するノードの更新
		//--------------------------------------------------------------------------------------
		class SelfAstarNodeController_Ex : public Component, public I_SelfAstarNodeRequester
		{
			std::weak_ptr<AstarNode> m_currentNode;				//現在のノード

			std::future<std::shared_ptr<AstarNode>> m_future;	//フューチャーデータ

		public:
			SelfAstarNodeController_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~SelfAstarNodeController_Ex() = default;

			void OnUpdate() override;

		private:

		public:
			//自分の位置ノードを検索する	
			std::shared_ptr<AstarNode> Search_SelfAstarNode() override;

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 現在のノードが存在するかどうかを判断する。
			/// </summary>
			/// <returns>現在のノード</returns>
			bool HasCurrentNode() const { return !m_currentNode.expired(); }

			/// <summary>
			/// 現在のノードを取得する。
			/// </summary>
			/// <returns>現在のノード</returns>
			std::shared_ptr<maru::AstarNode> GetCurrentNode() const;

		private:
			void SetCurrentNode(const std::shared_ptr<AstarNode>& node);

		};
	}
}