/*!
@file OpenDataHandler.cpp
@brief OpenDataHandlerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "OpenDataHandler.h"

#include "Heuristic.h"
#include "NavGraphNode.h"
#include "AstarEdge.h"
#include "AstarGraph.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	オープンデータ
	//--------------------------------------------------------------------------------------

	OpenData::OpenData()
		:OpenData(nullptr, nullptr, 0, 0)
	{}

	OpenData::OpenData(const std::shared_ptr<NavGraphNode>& parent, const std::shared_ptr<NavGraphNode>& node, const float range, const float heuristic)
		: parent(parent) ,node(node), range(range), heuristic(heuristic), isActive(true)
	{}

	float OpenData::GetSumRange() const {
		return range + heuristic;
	}

	bool OpenData::operator == (const OpenData& data) const {
		return this->node.lock() == data.node.lock();
	}

	//--------------------------------------------------------------------------------------
	///	オープンデータを扱う者。
	//--------------------------------------------------------------------------------------

	OpenDataHandler::OpenDataHandler() :
		m_heuristic(new Heuristic())
	{}

	/// <summary>
	/// オープンデータリストのソート条件式(合計値基準で昇順ソート式)
	/// </summary>
	/// <param name="left">レフトデータ</param>
	/// <param name="right">ライトデータ</param>
	/// <returns>左の合計値が小さいならtrue</returns>
	bool IsSmall_LeftOpenData(const std::shared_ptr<OpenData>& left, const std::shared_ptr<OpenData>& right) {
		return left->GetSumRange() < right->GetSumRange();	//合計値が小さい順に並べる。 
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSearchBaseOpenData(DataPtrList& openDataList) {
		openDataList.sort(&IsSmall_LeftOpenData);

		return openDataList.front();
	}

	bool OpenDataHandler::CreateOpenDatas(
		DataPtrList& openDataList,
		DataPtrList& closeDataList,
		const std::shared_ptr<OpenData>& openData,
		const std::shared_ptr<AstarGraph>& graph,
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode
	) {
		const auto& baseNode = openData->node.lock();		//基準となるノードを取得
		auto edges = graph->GetEdges(baseNode->GetIndex());	//エッジの取得

		openDataList.pop_front();			//使用するオープンデータを削除
		closeDataList.push_back(openData);	//使用するオープンデータをクローズリストに登録

		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());	//ノードの取得

			auto toNodeVec = node->GetPosition() - baseNode->GetPosition();			//ベースノードからの実コストを取得
			auto range = toNodeVec.length();
			range += openData->range;
			auto heuristicRange = m_heuristic->CalculateHeuristicRange(node);		//ヒュースリック距離の取得

			auto newData = std::make_shared<OpenData>(baseNode, node, range, heuristicRange);	//新規オープンデータの生成

			bool isAddData = AddOpenData(openDataList, closeDataList, newData);		//オープンデータの追加をする。

			//目標ノードなら終わらせる
			if (node == targetNode) {
				return true;
			}
		}

		return false;
	}

	bool OpenDataHandler::CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<NavGraphNode>& targetNode) {
		int index = 0;
		constexpr int tempMaxIndex = 10000;

		auto tempData = FindSomeOpenData(openDataList, targetNode);
		while (index <= tempMaxIndex) {
			if (tempData->parent.lock() == nullptr) {
				break;
			}

			m_route.push(tempData->node.lock());
			tempData = FindSomeOpenData(openDataList, tempData->parent.lock());
			index++;
		}

		return index <= tempMaxIndex;		//上限回数を超えたら、失敗
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<NavGraphNode>& node) {
		return FindSomeOpenData(dataList, std::make_shared<OpenData>(nullptr ,node, 0.0f, 0.0f));
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
		auto iter = dataList.begin();
		while (iter != dataList.end()) {
			if (*(*iter) == *openData) {
				return *iter;
			}
			iter++;
		}

		return nullptr;
	}

	bool OpenDataHandler::IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
		auto iter = dataList.begin();
		while (iter != dataList.end()) {
			if (*(*iter) == *openData) {
				return true;
			}
			iter++;
		}

		return false;
	}

	bool OpenDataHandler::AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData) {
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

	bool OpenDataHandler::StartSearchAstar(
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode,
		const std::shared_ptr<AstarGraph>& graph
	) {
		//オープンデータリストとクローズデータリストを生成
		auto openDataList = DataPtrList();
		auto closeDataList = DataPtrList();

		//初期オープンデータを生成
		m_heuristic->SetTargetNode(targetNode);
		openDataList.push_back(std::make_shared<OpenData>(nullptr, startNode, 0.0f, m_heuristic->CalculateHeuristicRange(startNode)));

		//オープンデータが存在する限りループする。
		while (openDataList.size() != 0) {	
			//オープンデータ生成用の基準ノードの生成。
			auto baseOpenData = FindSearchBaseOpenData(openDataList);

			//オープンデータの生成。ターゲットノードにたどり着いたらtrueを返す。
			if (CreateOpenDatas(openDataList, closeDataList, baseOpenData, graph, startNode, targetNode)) {
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

		bool isCreateRoute = CreateRoute(openDataList, targetNode);
		if (!isCreateRoute) {
			Debug::GetInstance()->Log(L"OpenDataHandler::SearchAstar() : Astarルートが検索できませんでした。");
		}

		return isSearchSuccess;
	}

	std::stack<std::shared_ptr<NavGraphNode>> OpenDataHandler::GetRoute() {
		return m_route;
	}



	//auto iter = m_closeDataMap.find(openData.node.lock()->GetIndex());	//削除したいイテレータの取得
	//m_closeDataMap.erase(iter);											//クローズマップから削除

}