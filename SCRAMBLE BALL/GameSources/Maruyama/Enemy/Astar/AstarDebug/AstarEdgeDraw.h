/*!
@file AstarEdgeDraw.h
@brief AstarEdgeDraw
担当者：丸山 裕喜
*/

#pragma once

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class AstarEdge;
	class ArrowObject;

	//--------------------------------------------------------------------------------------
	/// エッジデータ
	//--------------------------------------------------------------------------------------
	struct EdgeData {
		ex_weak_ptr<AstarEdge> astarEdge = nullptr;    //対応したエッジ
		ex_weak_ptr<GameObject> drawObject = nullptr;  //生成したエッジオブジェクト

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="astarEdge">対応したエッジ</param>
		/// <param name="drawObject">生成したエッジオブジェクト</param>
		EdgeData(const std::shared_ptr<AstarEdge>& astarEdge, const std::shared_ptr<GameObject>& drawObject);
	};

	//--------------------------------------------------------------------------------------
	/// Astarエッジ表示
	//--------------------------------------------------------------------------------------
	class AstarEdgeDraw : public Component {

		ex_weak_ptr<GraphAstar> m_astar;   //GraphAstarのポインタ
		std::vector<EdgeData> m_edgeDatas; //生成したエッジデータ

		/// <summary>
		/// エッジを生成する位置を計算して返す
		/// </summary>
		/// <param name="startPosition">ノードの開始位置</param>
		/// <param name="endPosition">ノードの終了位置</param>
		/// <returns>エッジを生成する位置</returns>
		Vec3 CalculateCreatePosition(const Vec3& startPosition, const Vec3& endPosition) const;

		/// <summary>
		/// エッジの大きさを計算して返す
		/// </summary>
		/// <param name="startPosition">ノードの開始位置</param>
		/// <param name="endPosition">ノードの終了位置</param>
		/// <returns>エッジを大きさ</returns>
		Vec3 CalculateCreateScale(const Vec3& startPosition, const Vec3& endPosition) const;

		/// <summary>
		/// 渡したオブジェクトの位置、向き、大きさをセッティングする。
		/// </summary>
		/// <param name="object">設定したいオブジェクト</param>
		/// <param name="startPosition">ノードの開始位置</param>
		/// <param name="endPosition">ノードの終了位置</param>
		/// <returns>エッジを大きさ</returns>
		void SettingDrawPositionRotateScale(const std::shared_ptr<GameObject>& object, const Vec3& startPosition, const Vec3& endPosition);

		/// <summary>
		/// エッジデータの生成
		/// </summary>
		/// <param name="object">生成したいエッジ</param>
		/// <param name="startPosition">ノードの開始位置</param>
		/// <param name="endPosition">ノードの終了位置</param>
		/// <returns>エッジデータ</returns>
		EdgeData CreateEdgeData(const std::shared_ptr<AstarEdge>& edge, const Vec3& startPosition, const Vec3& endPosition);

		/// <summary>
		/// ArrowObjectの生成
		/// </summary>
		/// <param name="position">生成したい場所</param>
		/// <param name="quat">生成したい向き</param>
		/// <returns>生成したArrowObjectのポインタ</returns>
		std::shared_ptr<ArrowObject> InstantiateArrowObject(const Vec3& position, const Quat& quat);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="astar">Astarグラフ</param>
		AstarEdgeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// 位置の更新
		/// </summary>
		void UpdatePosition();

		/// <summary>
		/// 引数で指定したエッジと一緒のオブジェクトを取得する。
		/// </summary>
		/// <param name="edge">見つけたいエッジ</param>
		std::shared_ptr<GameObject> FindEdgeDrawObject(const std::shared_ptr<AstarEdge>& edge);

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// デバッグ表示をするかどうか設定
		/// </summary>
		/// <param name="isDraw">デバッグ表示するかどうか</param>
		void SetDebugDraw(const bool isDraw);

		/// <summary>
		/// デバッグ表示状態の取得
		/// </summary>
		/// <returns>デバッグ表示状態</returns>
		bool GetDebugDraw() const;

	};

}

//endbasecross