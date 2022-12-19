
/*!
@file ImpactMap.cpp
@brief ImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/StateMachine/NodeBase.h"
#include "Maruyama/StateMachine/EdgeBase.h"
#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Factory_WayPointMap_FloodFill.h"

#include "Maruyama/Utility/UtilityObstacle.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace maru {

		using DirectionType = Factory_WayPointMap_FloodFill::DirectionType;
		using OpenData = Factory_WayPointMap_FloodFill::OpenData;

		/*
		//方向配列
		const Vec3 DIRECTIONS[] = {
			Vec3::Right(),										//右
			(Vec3::Right() + Vec3::Forward()).GetNormalized(),	//右奥
			(Vec3::Right() - Vec3::Forward()).GetNormalized(),	//右手前

			-Vec3::Right(),										//左
			(-Vec3::Right() + Vec3::Forward()).GetNormalized(),	//左奥
			(-Vec3::Right() - Vec3::Forward()).GetNormalized(),	//左手前

			Vec3::Forward(),									//奥
			-Vec3::Forward()									//手前
		};
		*/

		//方向マップ
		const std::unordered_map<DirectionType, Vec3> DIRECTION_MAP = {
			{ DirectionType::Right,			Vec3::Right() },										//右
			//{ DirectionType::RightForward,	(Vec3::Right() + Vec3::Forward()).GetNormalized()},		//右奥
			//{ DirectionType::RightBack,		(Vec3::Right() - Vec3::Forward()).GetNormalized()},		//右手前
			{ DirectionType::RightForward,	(Vec3::Right() + Vec3::Forward())},		//右奥
			{ DirectionType::RightBack,		(Vec3::Right() - Vec3::Forward())},		//右手前

			{ DirectionType::Left,			-Vec3::Right()},										//左
			//{ DirectionType::LeftForward,	(-Vec3::Right() + Vec3::Forward()).GetNormalized() },	//左奥
			//{ DirectionType::LeftBack,		(-Vec3::Right() - Vec3::Forward()).GetNormalized() },	//左手前
			{ DirectionType::LeftForward,	(-Vec3::Right() + Vec3::Forward()) },	//左奥
			{ DirectionType::LeftBack,		(-Vec3::Right() - Vec3::Forward()) },	//左手前

			{ DirectionType::Foward,		Vec3::Forward()},										//左奥
			{ DirectionType::Back,			-Vec3::Forward()},										//左手前
		};

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズムの生成用仮データ
		//--------------------------------------------------------------------------------------

		Factory_WayPointMap_FloodFill::OpenData::OpenData(
			const std::shared_ptr<AstarNode>& parentNode,
			const std::shared_ptr<AstarNode>& selfNode
		):
			parentNode(parentNode),
			selfNode(selfNode),
			isActive(true)
		{}

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム本体
		//--------------------------------------------------------------------------------------

		Factory_WayPointMap_FloodFill::Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage) :
			m_stage(stage)
		{
			/*
			auto startPosition = Vec3(0.0f);

			for (const auto& direction : DIRECTIONS) {
				auto targetPosition = startPosition + (direction * 2.0f);

				auto object = stage->Instantiate<GameObject>(targetPosition, Quat::Identity());
				object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
			}
			*/
		}

		std::unordered_map<DirectionType, int> Factory_WayPointMap_FloodFill::SettingIndexByDirection(const Parametor& parametor) {
			std::unordered_map<DirectionType, int> result;

			const maru::Rect& rect = parametor.rect;
			const float& intervalRange = parametor.intervalRange;

			//基準となる横の大きさと、縦の大きさ
			int widthCount = static_cast<int>(rect.width / intervalRange);
			int plusIndex = widthCount + 1;	//横の長さより一個分上で奥行き分のインデックスになる。

			result[DirectionType::Right] = +1;
			result[DirectionType::RightForward] = 1 + plusIndex;
			result[DirectionType::RightBack] = 1 - plusIndex;
			result[DirectionType::Left] = -1;
			result[DirectionType::LeftForward] = -1 + plusIndex;
			result[DirectionType::LeftBack] = -1 - plusIndex;
			result[DirectionType::Foward] = +plusIndex;
			result[DirectionType::Back] = -plusIndex;

			return result;
		}

		bool Factory_WayPointMap_FloodFill::IsNodeCreate(
			const std::shared_ptr<OpenData>& newOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor,
			bool& isRayHit
		) {
			std::lock_guard<mutex> lock(m_mutex);	//ミューテックスロック

			auto startPosition = newOpenData->parentNode.lock()->GetPosition();
			auto targetPosition = newOpenData->selfNode->GetPosition();

			//障害物に当たっていたら
			auto obstacleObjects = GetStage()->GetGameObjectVec();	//障害物配列
			if (isRayHit = maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, obstacleObjects)) {
				return false;	//生成できない
			}

			//ターゲットがエリアより外側にあるなら
			int testIndex = newOpenData->selfNode->GetIndex();
			auto selfPosition = newOpenData->selfNode->GetPosition();
			if (!parametor.rect.IsInRect(newOpenData->selfNode->GetPosition())) {
				return false;
			}

			//同じノードが存在するなら
			if (graph->IsSomeIndexNode(newOpenData->selfNode->GetIndex())) {
				return false;
			}

			return true;	//どの条件にも当てはまらないならtrue
		}


		bool Factory_WayPointMap_FloodFill::IsEdgeCreate(
			const std::shared_ptr<OpenData>& newOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor,
			const bool isRayHit
		) {
			//障害物に当たっているなら、生成しない
			if (isRayHit) {
				return false;
			}

			//ターゲットがエリアより外側にあるなら
			int testIndex = newOpenData->selfNode->GetIndex();
			auto selfPosition = newOpenData->selfNode->GetPosition();
			if (!parametor.rect.IsInRect(newOpenData->selfNode->GetPosition())) {
				return false;
			}

			//同じエッジが存在するなら
			const auto parentNode = newOpenData->parentNode.lock();
			const auto& selfNode = newOpenData->selfNode;
			if (graph->IsSomeIndexEdge(parentNode->GetIndex(), selfNode->GetIndex())) {
				return false;	//生成しない
			}

			return true;	//条件が通ったため、生成する。
		}

		void Factory_WayPointMap_FloodFill::CreateWayPoints(
			const std::shared_ptr<OpenData>& parentOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			auto openDatas = CreateChildrenOpenDatas(parentOpenData, parametor);	//オープンデータの生成

			//ループして、オープンデータの中から生成できるものを設定
			for (const auto& openData : openDatas) {
				auto parentNode = openData->parentNode.lock();
				auto selfNode = openData->selfNode;
				bool isRayHit = false;	//障害物に当たったかどうかを記録する。

				//ノードが生成できるなら
				if (IsNodeCreate(openData, graph, parametor, isRayHit)) {
					std::lock_guard<std::mutex> lock(m_mutex);
					auto node = graph->AddNode(openData->selfNode);	//グラフにノード追加
					m_openDataQueue.push(openData);					//生成キューにOpenDataを追加
				}

				//エッジの生成条件がそろっているなら
				if (IsEdgeCreate(openData, graph, parametor, isRayHit)) {
					auto edge = graph->AddEdge(parentNode->GetIndex(), selfNode->GetIndex());	//グラフにエッジ追加
				}
			}
		}

		int Factory_WayPointMap_FloodFill::CalculateIndex(
			const std::shared_ptr<OpenData>& parentOpenData,
			const DirectionType directionType
		) const {
			const auto& parent = parentOpenData->selfNode;
			int index = parent->GetIndex() + m_plusIndexMapByDirection.at(directionType);	//インデックスの計算
			return index;
		}

		std::vector<std::shared_ptr<OpenData>> Factory_WayPointMap_FloodFill::CreateChildrenOpenDatas(
			const std::shared_ptr<OpenData>& parentOpenData,
			const Parametor& parametor
		) {
			std::vector<std::shared_ptr<OpenData>> result;
			const auto& parent = parentOpenData->selfNode;	//親ノードを取得

			for (const auto& pair : DIRECTION_MAP) {
				const DirectionType& directionType = pair.first;	//方向タイプ
				const Vec3& direction = pair.second;				//方向ベクトル

				int index = CalculateIndex(parentOpenData, directionType);			//インデックスの計算
				if (index < 0) {	//インデックスが0より小さいなら処理を飛ばす。
					continue;
				}

				Vec3 startPosition = parent->GetPosition();			//開始位置
				Vec3 targetPosition = startPosition + (direction * parametor.intervalRange);	//生成位置

				auto newNode = std::make_shared<AstarNode>(index, targetPosition);	//新規ノードの作成

				auto newOpenData = std::make_shared<OpenData>(parent, newNode);		//新規データ作成
				result.push_back(newOpenData);
			}

			return result;
		}

		void Factory_WayPointMap_FloodFill::AddWayPointMap(
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			m_plusIndexMapByDirection = SettingIndexByDirection(parametor);	//方向別の加算するインデックス数をセッティング

			auto baseStartPosition = parametor.rect.CalculateStartPosition();

			maru::Utility::QueueClear(m_openDataQueue);
			auto newNode = std::make_shared<AstarNode>(0, baseStartPosition);
			//Debug::GetInstance()->Log(newNode->GetPosition());
			graph->AddNode(newNode);
			m_openDataQueue.push(std::make_shared<OpenData>(nullptr, newNode));

			while (!m_openDataQueue.empty()) {	//キューが空になるまで
				auto parentData = m_openDataQueue.front();
				m_openDataQueue.pop();
				CreateWayPoints(parentData, graph ,parametor);
			}
		}

	}
}