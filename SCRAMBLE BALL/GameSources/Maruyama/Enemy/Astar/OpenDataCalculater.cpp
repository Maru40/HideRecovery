/*!
@file OpenDataCalculater.cpp
@brief OpenDataCalculaterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarNode.h"
#include "AstarEdge_Ex.h"
#include "OpenDataCalculater.h"

#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		//--------------------------------------------------------------------------------------
		///	オープンデータ本体
		//--------------------------------------------------------------------------------------

		OpenData::OpenData(
			const std::shared_ptr<AstarNode>& parentNode,
			const std::shared_ptr<AstarNode>& selfNode,
			const float range,
			const float heuristic
		):
			parentNode(parentNode),
			selfNode(selfNode),
			range(range),
			heuristic(heuristic),
			isActive(true)
		{}

		float OpenData::GetSumRange() const { return range + heuristic; }

		bool OpenData::operator == (const OpenData& data) const {
			return this->selfNode.lock() == data.selfNode.lock();
		}

		//--------------------------------------------------------------------------------------
		///	オープンデータ計算クラス本体
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// オープンデータリストのソート条件式(合計値基準で昇順ソート式)
		/// </summary>
		/// <param name="left">レフトデータ</param>
		/// <param name="right">ライトデータ</param>
		/// <returns>左の合計値が小さいならtrue</returns>
		bool IsSmall_LeftOpenData(const std::shared_ptr<OpenData>& left, const std::shared_ptr<OpenData>& right) {
			if (!left->isActive) {	//非アクティブなら最後尾にする。
				return false;
			}

			return left->GetSumRange() < right->GetSumRange();	//合計値が小さい順に並べる。
		}

		OpenDataCalculater::OpenDataCalculater() {}

		float OpenDataCalculater::CalculateHeuristicRange(
			const std::shared_ptr<AstarNode> currentNode,
			const std::shared_ptr<AstarNode> targetNode
		) const {
			auto position = currentNode->GetPosition();
			auto targetPosition = targetNode->GetPosition();

			auto toVec = targetPosition - position;
			return toVec.length();
		}

		std::shared_ptr<OpenData> OpenDataCalculater::FindSearchBaseOpenData(DataPtrList& openDataList) {
			openDataList.sort(&IsSmall_LeftOpenData);

			return openDataList.front();
		}

		bool OpenDataCalculater::IsOhterAreaTarget(const std::shared_ptr<AstarNode>& startNode, const int targetAreaIndex) {
			//目標エリアが0以上、かつ、目標ノードが目標エリアと違うとき。
			return false;	//仮でfalseを返している状態
			//return (targetAreaIndex >= 0 && targetAreaIndex != startNode->GetAreaIndex());
		}

		bool OpenDataCalculater::CreateOpenDatas(
			DataPtrList& openDataList,
			DataPtrList& closeDataList,
			const std::shared_ptr<OpenData>& openData,
			const std::shared_ptr<GraphType>& graph,
			const std::shared_ptr<AstarNode>& startNode,
			const std::shared_ptr<AstarNode>& targetNode,
			const int targetAreaIndex
		) {
			const auto& baseNode = openData->selfNode.lock();		//基準となるノードを取得
			
			auto edges = graph->GetEdges(baseNode->GetIndex());	//エッジの取得
			int baseIndex = baseNode->GetIndex();

			openDataList.pop_front();			//使用するオープンデータを削除
			closeDataList.push_back(openData);	//使用するオープンデータをクローズリストに登録

			std::vector<std::shared_ptr<OpenData>> otherAreaOpenDatas;

			for (auto& edge : edges) {
				auto node = graph->GetNode(edge->GetToIndex());	//ノードの取得
				if (!node) {
					auto strIndex = std::to_wstring(edge->GetToIndex());
					Debug::GetInstance()->Log(L"OpenDataHandler::CreateOpenData(): ノードがnullです。: " + strIndex);
					continue;
				}

				auto toNodeVec = node->GetPosition() - baseNode->GetPosition();			//ベースノードからの実コストを取得
				auto range = toNodeVec.length();
				range += openData->range;
				auto heuristicRange = CalculateHeuristicRange(node, targetNode);		//ヒュースリック距離の取得

				int newIndex = node->GetIndex();
				auto newData = std::make_shared<OpenData>(baseNode, node, range, heuristicRange);	//新規オープンデータの生成

				bool isAddData = AddOpenData(openDataList, closeDataList, newData);		//オープンデータの追加をする。

				//目標ノードなら終わらせる
				if (node == targetNode) {
					return true;
				}

				//目標エリアが他エリアで、かつ、目標エリアのノードならotherAreaOpenDatasに登録する。
				//if (IsOhterAreaTarget(startNode, targetAreaIndex) && node->GetAreaIndex() == targetAreaIndex) {
				//	otherAreaOpenDatas.push_back(newData);
				//}
			}

			//別エリアのデータが存在するなら、別エリアとの境目で探索終了
			if (!otherAreaOpenDatas.empty()) {
				std::sort(otherAreaOpenDatas.begin(), otherAreaOpenDatas.end(), &IsSmall_LeftOpenData);
				m_otherAreaNode = otherAreaOpenDatas[0]->selfNode.lock();
				return true;
			}

			return false;
		}


		std::stack<std::weak_ptr<AstarNode>> OpenDataCalculater::CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<AstarNode>& targetNode) {
			int index = 0;
			constexpr int tempMaxIndex = 10000;

			std::stack<std::weak_ptr<AstarNode>> result;

			auto tempData = FindSomeOpenData(openDataList, targetNode);
			while (index <= tempMaxIndex) {
				if (tempData->parentNode.lock() == nullptr) {
					break;
				}

				result.push(tempData->selfNode.lock());
				tempData = FindSomeOpenData(openDataList, tempData->parentNode.lock());
				index++;
			}

			bool isCreateRoute = index <= tempMaxIndex;		//上限回数を超えたら、失敗
			if (!isCreateRoute) {
				Debug::GetInstance()->Log(L"OpenDataHandler::SearchAstar() : Astarルートが検索できませんでした。");
			}

			return result;
		}

		std::shared_ptr<OpenData> OpenDataCalculater::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<AstarNode>& node) {
			return FindSomeOpenData(dataList, std::make_shared<OpenData>(nullptr, node, 0.0f, 0.0f));
		}

		std::shared_ptr<OpenData> OpenDataCalculater::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
			auto iter = dataList.begin();
			while (iter != dataList.end()) {
				if (*(*iter) == *openData) {
					return *iter;
				}
				iter++;
			}

			return nullptr;
		}

		bool OpenDataCalculater::IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
			auto iter = dataList.begin();
			while (iter != dataList.end()) {
				if (*(*iter) == *openData) {
					return true;
				}
				iter++;
			}

			return false;
		}

		bool OpenDataCalculater::AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData) {
			auto someOpenData = FindSomeOpenData(openDataList, openData);	//オープンデータリストからオープンデータと同じデータを探す。
			auto someCloseData = FindSomeOpenData(closeDataList, openData);	//クローズデータリストからオープンデータと同じデータを探す。

			//オープンリストに登録されていない、かつ、クローズリストに登録されていない。
			if (!someOpenData && !someCloseData) {
				openDataList.push_back(openData);	//オープンデータに追加
				return true;
			}

			//オープンリストに登録されていて、新規データの方が小さい合計値
			if (someOpenData && !IsSmall_LeftOpenData(someOpenData, openData)) {
				(*someOpenData) = (*openData);		//オープンデータの中身を更新
				return true;
			}

			//クローズリストに登録されていて、新規データの方が小さい合計値
			if (someCloseData && !IsSmall_LeftOpenData(someCloseData, openData)) {
				openDataList.push_back(openData);		//オープンデータに登録
				closeDataList.remove(someCloseData);	//クローズデータから削除
				return true;
			}

			//どの条件にも当てはまらないなら、追加をしない。
			return false;
		}
		
		bool OpenDataCalculater::StartSearchAstar(
			const std::shared_ptr<AstarNode>& startNode,
			const std::shared_ptr<AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph,
			const int targetAreaIndex
		) {
			maru::Utility::StackClear(m_route);
			m_otherAreaNode.reset();

			//オープンデータリストとクローズデータリストを生成
			auto openDataList = DataPtrList();
			auto closeDataList = DataPtrList();

			//初期オープンデータを生成
			openDataList.push_back(std::make_shared<OpenData>(nullptr, startNode, 0.0f, CalculateHeuristicRange(startNode, targetNode)));

			//オープンデータが存在する限りループする。
			while (openDataList.size() != 0) {
				//オープンデータ生成用の基準ノードの生成。
				auto baseOpenData = FindSearchBaseOpenData(openDataList);

				//オープンデータの生成。ターゲットノードにたどり着いたらtrueを返す。
				if (CreateOpenDatas(openDataList, closeDataList, baseOpenData, graph, startNode, targetNode, targetAreaIndex)) {
					break;
				}
			}

			bool isSearchSuccess = (openDataList.size() != 0);	//オープンデータが存在するなら、検索成功

			if (!isSearchSuccess) {	//サーチが失敗したら処理を飛ばす。
				return isSearchSuccess;
			}

			//クローズデータをオープンデータに含める。
			for (auto closeData : closeDataList) {
				openDataList.push_back(closeData);
			}

			auto lastTargetNode = m_otherAreaNode.lock() ? m_otherAreaNode.lock() : targetNode;	//最終的なターゲットノード
			m_route = CreateRoute(openDataList, lastTargetNode);
			
			return isSearchSuccess;
		}

		std::stack<std::weak_ptr<AstarNode>> OpenDataCalculater::GetRoute() {
			return m_route;
		}

	}
}