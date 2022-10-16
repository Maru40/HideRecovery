
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
	class NavGraphNode;
	class ImpactDataDraw;
	class ImpactDataDrawObject;

	//--------------------------------------------------------------------------------------
	///	デバッグ表示用のデータ
	//--------------------------------------------------------------------------------------
	struct DrawNodeData {
		std::weak_ptr<NavGraphNode> node;					//ノード
		std::shared_ptr<NumbersObject> nodeNumber;			//数字表記
		std::shared_ptr<ImpactDataDrawObject> impactData;	//影響データの表示管理クラス

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="node">ノードのポインタ</param>
		DrawNodeData(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// ノードが存在するかどうか
		/// </summary>
		/// <returns>ノードが存在するならtrue</returns>
		bool HasNode() const noexcept { return !node.expired(); }

		/// <summary>
		/// データの更新呼び出し
		/// </summary>
		void OnUpdate();
		
		/// <summary>
		/// データの表示更新呼び出し
		/// </summary>
		void OnDraw();

		/// <summary>
		/// データのアクティブの設定
		/// </summary>
		/// <param name="isActive">データのアクティブ状態</param>
		void SetDrawActive(const bool isActive);
	};

	//--------------------------------------------------------------------------------------
	///	Astarノード表示
	//--------------------------------------------------------------------------------------
	class AstarNodeDraw : public Component
	{
		ex_weak_ptr<GraphAstar> m_astar;		//Astarグラフ

		std::vector<DrawNodeData> m_drawDatas;	//生成した表示用データ

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
		void OnDestroy() override;

	private:
		/// <summary>
		/// 表示データをAstarのノード分生成
		/// </summary>
		void CreateDrawDatas();

		/// <summary>
		/// ノード番号表示オブジェクトの生成
		/// </summary>
		/// <param name="node">生成したいノード</param>
		/// <returns>生成したオブジェクト</returns>
		std::shared_ptr<NumbersObject> CreateNodeNumber(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// 影響データ表示オブジェクトの生成
		/// </summary>
		/// <param name="node">生成したいノード</param>
		/// <returns>生成したオブジェクト</returns>
		std::shared_ptr<ImpactDataDrawObject> CreateImpactDataDraw(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// ノードの更新
		/// </summary>
		void UpdateNodes();

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