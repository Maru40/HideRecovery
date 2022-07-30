
/*!
@file AstarNodeDraw.h
@brief AstarNodeDrawクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class NumbersObject;

	//--------------------------------------------------------------------------------------
	///	Astarノード表示
	//--------------------------------------------------------------------------------------
	class AstarNodeDraw : public Component
	{
		ex_weak_ptr<GraphAstar> m_astar;				//Astarグラフ

		vector<std::shared_ptr<NumbersObject>> m_nodes;	//生成したノードオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="astar">Astarグラフ</param>
		AstarNodeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// ノードの生成
		/// </summary>
		void CreateNodeDraw();

		/// <summary>
		/// ノードの位置更新
		/// </summary>
		void UpdateNodesPosition();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// デバッグ表示設定
		/// </summary>
		/// <param name="isDraw">デバッグ表示するかどうか</param>
		void SetDebugDraw(const bool isDraw);

		/// <summary>
		/// デバッグ表示するかどうか
		/// </summary>
		/// <returns>デバッグ表示をするならtrue</returns>
		bool GetDebugDraw() const;

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="nodeIndex">設定したいノードのインデックス</param>
		/// <param name="color">色</param>
		void SetColor(const int& nodeIndex, const Col4& color);
	};

}

//endbasecorss