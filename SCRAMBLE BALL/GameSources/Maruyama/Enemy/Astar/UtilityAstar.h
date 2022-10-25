/*!
@file UtilityAstar.h
@brief UtilityAstarなど
担当者：丸山 裕喜
*/

#pragma once

#include "NavGraphNode.h"
#include "AstarEdge.h"

#include "GraphAstar.h"

#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astarでよく使う計算をまとめたクラス
	/// GraphAstarが余りにも関数が増えたたため、計算系を分けるために生成
	//--------------------------------------------------------------------------------------
	class UtilityAstar
	{
	public:

		/// <summary>
		/// ターゲットから一番近くのノードを検索する。
		/// </summary>
		/// <param name="astar">Astarの参照</param>
		/// <param name="target">ターゲット</param>
		/// <returns>ターゲットの一番近くのノード</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(const GraphAstar& astar, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットから一番近くのノードを検索する。
		/// </summary>
		/// <param name="astar">Astarの参照</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		/// <param name="excluteObjs">Rayの対象外にするオブジェクト群</param>
		/// <returns>ターゲットから一番近いノード</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(const GraphAstar& astar, const Vec3& targetPos);

		/// <summary>
		/// ターゲットから一番近くのノードを検索する。
		/// </summary>
		/// <param name="graph">グラフの参照</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		/// <param name="isObstacleConfirmation">障害物判定を行うかどうか</param> 
		/// <returns>ターゲットから一番近いノード</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(const std::shared_ptr<const GraphAstar::GraphType>& graph, const Vec3& targetPos, const bool isObstacleConfirmation = true);

		/// <summary>
		/// 自分のノードから伸びるノードで一番ターゲットに近いノードの選択
		/// </summary>
		/// <param name="astar">Astarの参照</param>
		/// <param name="selfObject">自分自身</param>
		/// <param name="target">ターゲット</param>
		/// <returns>一番近いノード</returns>
		static std::shared_ptr<NavGraphNode> SearchMyNodeToTargetNearNode(const GraphAstar& astar,
			const std::shared_ptr<GameObject>& selfObject,const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// 自分のノードからターゲット方向にあるノードを検索
		/// </summary>
		/// <param name="astar">Astarの参照</param>
		/// <param name="startNode">開始ノード</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		/// <returns>ターゲット方向のノード</returns>
		static std::shared_ptr<NavGraphNode> CalculateTargetDirectNode(const GraphAstar& astar,
			const std::shared_ptr<NavGraphNode>& startNode,
			const Vec3& targetPos);

		/// <summary>
		/// ターゲットから逃げるための方向のノードを検索する
		/// </summary>
		/// <param name="astar">Astarの参照</param>
		/// <param name="startNode">開始ノード</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		/// <returns>ターゲット方向のノード</returns>
		static std::shared_ptr<NavGraphNode> CalculateTargetEscapeDirectNode(const GraphAstar& astar,
			const std::shared_ptr<NavGraphNode>& startNode,
			const Vec3& targetPos);

		/// <summary>
		/// 直進上で最も近いノードかどうか
		/// </summary>
		/// <param name="graph">グラフクラス</param>
		/// <param name="toVec">目的の方向</param>
		/// <param name="newNode">新規ノード</param>
		/// <param name="compareNode">比較対象のノード</param>
		/// <returns>最も近いならtrue</returns>
		template<class NodeType, class EdgeType>
		static bool IsNearNode(const std::shared_ptr<SparseGraph<NodeType, EdgeType>>& graph,
			Vec3 toVec,
			const std::shared_ptr<NodeType>& newNode,
			const std::shared_ptr<NodeType>& compareNode
		)
		{
			auto nodes = graph->GetNodes();
			for (const auto& node : nodes) {
				if (node == newNode || node == compareNode) { //同じなら処理をしない
					continue;
				}

				auto newToVec = node->GetPosition() - newNode->GetPosition();

				const float NearAngle = XMConvertToRadians(10.0f);
				auto newDot = dot(toVec.GetNormalized(), newToVec.GetNormalized());
				auto newRad = (acosf(newDot));

				//ほぼ同じ方向かつ、短いなら
				if (newRad < NearAngle && toVec.length() > newToVec.length()) {
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// 新規ノードからエッジの生成
		/// </summary>
		/// <param name="graph">グラフクラス</param>
		/// <param name="newNode">新規ノード</param>
		/// <param name="isRayHit">レイの確認をするかどうか</param>
		/// <returns>生成したエッジ群</returns>
		template<class NodeType, class EdgeType>
		static vector<std::shared_ptr<EdgeType>> CreateAdjacendEdges(
			const std::shared_ptr<GraphAstar::GraphType>& graph,
			const std::shared_ptr<NodeType>& newNode,
			const bool isRayHit = true
		) 
		{
			auto nodes = graph->GetNodes();
			vector<std::shared_ptr<EdgeType>> resultEdge;

			//八方向に近いオブジェクトがあるなら処理をしない
			//constexpr float NearRange = 2.0f;
			//Vec3 positions[] = {
			//	newNode->GetPosition() + ( Vec3::Forward() * NearRange),
			//	newNode->GetPosition() + (-Vec3::Forward() * NearRange),
			//	newNode->GetPosition() + ( Vec3::Right() * NearRange),
			//	newNode->GetPosition() + (-Vec3::Right() * NearRange),
			//};

			//for (auto& position : positions) {
			//	auto objects = maru::Utility::GetStage()->GetGameObjectVec();
			//	if (maru::UtilityObstacle::IsRayObstacle(newNode->GetPosition(), position, objects)) {
			//		return resultEdge;
			//	}
			//}


			for (const auto& node : nodes) {
				if (node == newNode) {  //同じなら処理をしない
					continue;
				}

				auto toVec = node->GetPosition() - newNode->GetPosition();
				if (!IsNearNode<NodeType, EdgeType>(graph, toVec, newNode, node)) { //近いノードでなかったら
					continue;
				}

				//障害物に当たっていなかったら
				auto objects = maru::Utility::GetStage()->GetGameObjectVec();
				if( !isRayHit ||
					(isRayHit && !maru::UtilityObstacle::IsRayObstacle(newNode->GetPosition(), node->GetPosition(), objects))
				) {
					auto fromEdge = std::make_shared<EdgeType>(newNode->GetIndex(), node->GetIndex());

					graph->AddEdge(fromEdge);
					resultEdge.push_back(fromEdge);
				}
			}

			return resultEdge;
		}

		/// <summary>
		/// nodeのインデックスから隣接するエッジの生成する。
		/// 生成する場合に直接渡されたSparseGraphにAddする。
		/// </summary>
		/// <param name="graph">Graphの参照</param>
		/// <param name="newNode">新規ノード</param>
		/// <param name="obstacleObjs">障害物のオブジェクト配列</param>
		/// <param name="excluteObjs">障害物から省くオブジェクト配列</param>
		/// <returns>生成されたエッジの配列</returns>
		template<class NodeClass, class EdgeClass>
		static vector<std::shared_ptr<AstarEdge>> CreateAdjacendEdges(std::shared_ptr<GraphAstar::GraphType>& graph, const std::shared_ptr<NavGraphNode>& newNode,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			//使わない。
			auto nodes = graph->GetNodes();

			vector<std::shared_ptr<AstarEdge>> reEdges;
			for (auto& node : nodes) {

				//障害物がなかったらエッジを追加する。
				if (!maru::Utility::IsRayObstacle(newNode->GetPosition(), node->GetPosition(), obstacleObjs, excluteObjs)) {
					//双方向にエッジを生成
					graph->AddEdge(std::make_shared<AstarEdge>(newNode->GetIndex(), node->GetIndex()));
					graph->AddEdge(std::make_shared<AstarEdge>(node->GetIndex(), newNode->GetIndex()));
					//どのようなエッジを生成したか返せるように別の配列に入れる。
					reEdges.push_back(std::make_shared<AstarEdge>(newNode->GetIndex(), node->GetIndex()));
					reEdges.push_back(std::make_shared<AstarEdge>(node->GetIndex(), newNode->GetIndex()));
				}
			}

			return reEdges;
		}

	};

}