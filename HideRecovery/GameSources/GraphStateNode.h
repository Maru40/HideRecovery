
/*!
@file GraphStateNode.h
@brief GraphStateNode
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class I_StateNode;

	//--------------------------------------------------------------------------------------
	///	ステート用のグラフノードクラス
	//--------------------------------------------------------------------------------------
	class GraphStateNode : public GraphNodeBase
	{
	private:
		std::shared_ptr<I_StateNode> m_stateNode = nullptr;  //ステートノード

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stateNode">ノードのインターフェース</param>
		GraphStateNode(const std::shared_ptr<I_StateNode>& stateNode);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">ノードのインデックス</param>
		/// <param name="stateNode">ノードのインターフェース</param>
		GraphStateNode(const int& index, const std::shared_ptr<I_StateNode>& stateNode);

		virtual void OnStart();
		virtual void OnUpdate();
		virtual void OnExit();

	};

}