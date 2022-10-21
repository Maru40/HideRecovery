
/*!
@file AstarNavGraphNode.h
@brief AstarNavGraphNodeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "NavGraphNode.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astar用のナビグラフノード
	//--------------------------------------------------------------------------------------
	class AstarNavGraphNode : public NavGraphNode
	{
		std::shared_ptr<GraphAstar> m_astar;	//Astarグラフ

	public:

		AstarNavGraphNode();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		AstarNavGraphNode(const int& index);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		AstarNavGraphNode(const int& index, const Vec3& position);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		/// <param name="impactData">影響マップデータ</param>
		AstarNavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		/// <param name="impactData">影響マップデータ</param>
		/// <param name="parent">親オブジェクト</param>
		AstarNavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="node">コピーするノード</param>
		AstarNavGraphNode(const AstarNavGraphNode& node);

		virtual ~AstarNavGraphNode() = default;

	public:

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// Astarの設定
		/// </summary>
		/// <param name="astar">Astar</param>
		void SetGraphAstar(const std::shared_ptr<GraphAstar>& astar) noexcept;

		/// <summary>
		/// Astarの取得
		/// </summary>
		/// <returns>Astarの取得</returns>
		std::shared_ptr<GraphAstar> GetGraphAstar() const noexcept;
	};

}