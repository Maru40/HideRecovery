
/*!
@file ImpactMap.h
@brief ImpactMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class AstarNodeDraw;
	class AstarEdgeDraw;
	class NavGraphNode;
	class I_Impacter;

	struct EyeSearchRangeParametor;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	四角形データ(生成する範囲に利用)
		//--------------------------------------------------------------------------------------
		struct Rect
		{
			Vec3 startPosition;   //開始場所
			Vec3 centerPosition;  //中心位置
			float width;          //横のサイズ
			float depth;          //奥行のサイズ

			Rect();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="centerPosition">中心位置</param>
			/// <param name="width">横の長さ</param>
			/// <param name="depth">奥行の長さ</param>
			Rect(const Vec3& centerPosition, const float& width, const float& depth);

			/// <summary>
			/// 開始位置を取得
			/// </summary>
			/// <returns>開始位置</returns>
			Vec3 CalculateStartPosition() const;

			/// <summary>
			/// 四角形内にいるかどうか
			/// </summary>
			/// <returns>四角形内ならtrue</returns>
			bool IsInRect(const Vec3& position);
		};


		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ用パラメータ
		//--------------------------------------------------------------------------------------
		struct Factory_ImpactMap_Parametor 
		{
			//--------------------------------------------------------------------------------------
			///	エリア分けのノード数構造体
			//--------------------------------------------------------------------------------------
			struct AreaNodeCount
			{
				int width;	//横のノード数
				int depth;	//縦のノード数

				AreaNodeCount();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="width">横のノード数</param>
				/// <param name="depth">縦のノード数</param>
				AreaNodeCount(const int width, const int depth);
			};

			//--------------------------------------------------------------------------------------
			///	メンバー変数
			//--------------------------------------------------------------------------------------

			Rect rect;						//範囲の四角形データ
			float intervalRange;			//ウェイポイントを置く間隔
			AreaNodeCount areaNodeCount;	//エリアの区切りのノード数
			float createHeight;				//生成する場所の高さ

			Factory_ImpactMap_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="rect">生成する四角形範囲</param>
			/// <param name="intervalRange">ウェイポイントを置く間隔</param>
			Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="rect">生成する四角形範囲</param>
			/// <param name="intervalRange">ウェイポイントを置く間隔</param>
			/// <param name="areaRangeCount">エリア区切りのノード数</param>
			Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange, const AreaNodeCount areaNodeCount);
		};

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ
		//--------------------------------------------------------------------------------------
		class Factory_ImpactMap
		{
		public:
			using Parametor = Factory_ImpactMap_Parametor;

		private:
			Parametor m_param; //パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="parametor">パラメータ</param>
			Factory_ImpactMap(const Parametor& parametor);

		private:

			/// <summary>
			/// エリアインデックスの計算
			/// </summary>
			/// <param name="widthCount">横のカウント</param>
			/// <param name="depthCount">縦のカウント</param>
			/// <returns>エリアインデックス</returns>
			int CalculateAreaIndex(const int widthCount, const int depthCount);
			
			/// <summary>
			/// ノードを全て生成
			/// </summary>
			/// <param name="astar">ノードを生成するGraphAstarのポインタ</param>
			void CreateNodes(const std::shared_ptr<GraphAstar>& astar);

			/// <summary>
			/// エッジの生成
			/// </summary>
			/// <param name="astar">エッジを生成するGraphAstarのポインタ</param>
			void CreateEdges(const std::shared_ptr<GraphAstar>& astar);

		public:
			/// <summary>
			/// 影響マップ用のグラフを生成
			/// </summary>
			/// <returns>影響マップ用のグラフ</returns>
			std::shared_ptr<GraphAstar> CreateGraphAstar();

			/// <summary>
			/// 影響マップ用のグラフを生成
			/// </summary>
			/// <param name="parametor">生成用パラメータ</param>
			/// <returns>生成したグラフ</returns>
			std::shared_ptr<GraphAstar> CreateGraphAstar(const Parametor& parametor);

			/// <summary>
			/// 影響マップにノードを追加する。
			/// </summary>
			/// <param name="astar">影響マップのグラフデータ</param>
			/// <param name="parametor">生成用のパラメータ</param>
			void AddNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// 現在のノードに合わせて影響マップのエッジを生成する。
			/// </summary>
			/// <param name="astar">影響マップのグラフデータ</param>
			/// <param name="parametor">生成用のパラメータ</param>
			void AddEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// エッジとノードの両方を生成する。
			/// </summary>
			/// <param name="astar">影響マップのグラフデータ</param>
			/// <param name="parametor">生成用パラメータ</param>
			void AddNodesEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

		};

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム(現在作成中) 
		//--------------------------------------------------------------------------------------
		class Factory_ImpactMap_FloodFill
		{
			struct Parametor {
				float intervalRange = 5.0f;  //ノードの間隔距離
				int numLoopCount = 3;        //生成ループ回数
			};

		public:
			//void AddImpactMap(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);
		};


		//--------------------------------------------------------------------------------------
		///	影響マップ本体
		//--------------------------------------------------------------------------------------
		class ImpactMap
		{
		public:
			using Factory_Parametor = Factory_ImpactMap_Parametor;
			using GraphAstarMap = std::unordered_map<int, std::shared_ptr<GraphAstar>>;
			

		private:
			Factory_Parametor m_param = Factory_Parametor();	//パラメータ

			std::weak_ptr<Stage> m_stage;						//自分の所属するステージ。
			//std::shared_ptr<GraphAstar> m_areaGraphAstar;		//エリア間のグラフ管理クラス
			//std::shared_ptr<GraphAstar> m_inAreaGraphAstar;		//エリア内の細かいグラフ管理クラス

			std::shared_ptr<GraphAstar> m_baseAstar;			//Astar管理をするグラフを渡す。(一番親のグラフ)
			//GraphAstarMap m_childAstarMap;						//Astarの子クラスを管理するクラス。

			//GraphAstarMap m_astarMap;	//グラフのマップ(0をエリア、1をエリア内、2をさらに細かく分けた場合などに使う。)

			//デバック用----------------------------------------------------------------

			ex_weak_ptr<AstarNodeDraw> m_nodeDraw = nullptr;	//ノードの見た目
			ex_weak_ptr<AstarEdgeDraw> m_edgeDraw = nullptr;	//エッジの見た目

			//--------------------------------------------------------------------------

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">自分が所属するステージのポインタ</param>
			ImpactMap(const std::shared_ptr<Stage>& stage);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">自分が所属するステージのポインタ</param>
			/// <param name="parametor">パラメータ</param>
			ImpactMap(const std::shared_ptr<Stage>& stage, const Factory_Parametor& parametor);

			/// <summary>
			/// デストラクタ
			/// </summary>
			~ImpactMap();

		public:

			/// <summary>
			/// 影響マップデータを作成する。
			/// </summary>
			void CreateImpactData();

			/// <summary>
			/// 影響マップデータを作成する。
			/// </summary>
			/// <param name="parametor">影響マップ生成用のパラメータ</param>
			void CreateImpactData(const Factory_Parametor& parametor);

			/// <summary>
			/// ノードの複数追加生成
			/// </summary>
			/// <param name="parametor">影響マップ生成用のパラメータ</param>
			void AddNodes(const Factory_Parametor& parametor);

			/// <summary>
			/// エッジの複数追加生成
			/// </summary>
			/// <param name="parametor">影響マップ生成用のパラメータ</param>
			void AddEdges(const Factory_Parametor& parametor);

			/// <summary>
			/// ノードとエッジを追加生成する。
			/// </summary>
			/// <param name="parametor">影響マップ生成用のパラメータ</param>
			void AddImpactData(const Factory_Parametor& parametor);

			/// <summary>
			/// ノードのクリア
			/// </summary>
			void ClearNodes();

			/// <summary>
			/// エッジのクリア
			/// </summary>
			void ClearEdges();

			/// <summary>
			/// 検索したルートを全て取得する。
			/// </summary>
			/// <param name="selfPosition">自分自身のポジション</param>
			/// <param name="targetPosition">目的地</param>
			/// <returns>検索したポジションの配列</returns>
			std::vector<Vec3> GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition);

			/// <summary>
			/// 検索したルートを全て取得する。
			/// </summary>
			/// <param name="selfPosition">自分自身のポジション</param>
			/// <param name="targetPosition">目的地</param>
			/// <param name="areaIndex">エリアインデックス</param>
			/// <returns>検索したルートのポジション</returns>
			std::vector<Vec3> GetRoutePositions(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex);

			/// <summary>
			/// 自分の視界内のノードを取得
			/// </summary>
			/// <param name="selfPosition">自分自身の位置</param>
			/// <param name="impacter">影響を与える者</param>
			/// <returns></returns>
			std::vector<std::shared_ptr<NavGraphNode>> GetEyeRangeNodes(const Vec3& selfPosition, const std::shared_ptr<I_Impacter>& impacter);

			//------------------------------------------------------------------------------------------------
			/// アクセッサ
			//------------------------------------------------------------------------------------------------

			/// <summary>
			/// 生成用のパラメータをセット
			/// </summary>
			/// <param name="parametor">生成用のパラメータ</param>
			void SetFactoryParametor(const Factory_Parametor& parametor) noexcept;

			/// <summary>
			/// 生成用のパラメータの取得
			/// </summary>
			/// <returns></returns>
			Factory_Parametor GetFactoryParametor() const noexcept;

			/// <summary>
			/// Astar用のグラフを取得(将来的に削除)
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<GraphAstar> GetGraphAstar() const noexcept;

			/// <summary>
			/// ステージの取得
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }


		private:
			//デバッグ系----------------------------------------------------------------------------------------

		public:
			//表示の切り替えなど
			void DebugInput(const std::function<bool()>& isInput);

			//デバッグ用のDrawを生成
			void CreateDebugDraw(const bool isDraw = true);

			//デバッグDrawの表示切り替え
			void SetIsDebugDraw(const bool isDraw);

			//ターゲットにしたノードの色を変更する。
			void ChangeTargetNodeColor(const int& nodeIndex, const Col4& color);

		};

	}
}