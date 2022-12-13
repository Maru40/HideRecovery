
/*!
@file CellMapImpactManager.cpp
@brief CellMapImpactManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CellMapImpactManager.h"

#include "Maruyama/Thread/ThreadPool.h"
#include "Maruyama/Enemy/ImpactMap/CellMap.h"

namespace basecross {

	namespace maru {

		CellMapImpactController::CellMapImpactController(
			const std::shared_ptr<CellMap>& cellMap,
			const std::uint_fast32_t& thread_count
		) :
			m_cellMap(cellMap),
			m_threadPool(new ThreadPool(thread_count))
		{ }

		CellMapImpactController::Cells CellMapImpactController::ThreadWoker_CalculateImpactCells(
			const std::weak_ptr<GameObject>& requester, 
			const std::shared_ptr<FutureData>& futureData
		) {
			auto result = Cells();

			//影響度の計算------
			
					//自分のセルの場所を検索。(別で常に監視してるデータを将来的に用意)
					//自分の周りの影響度をあげる。
					//視界に合わせて影響度を調整。
					//障害物に合わせて影響度を調整。

			//------------------
			
			futureData->m_isRunning = false;
			return result;
		}

		std::shared_ptr<CellMapImpactController::FutureData> CellMapImpactController::AddTask(const std::weak_ptr<GameObject>& requester) {
			auto futureData = std::make_shared<FutureData>();	//フューチャーデータの生成。
			auto future = m_threadPool->Submit(&CellMapImpactController::ThreadWoker_CalculateImpactCells, this, requester, futureData);	//スレッドにサブミット。
			futureData->MoveFuture(future);		//フューチャーの設定。
			return futureData;	
		}

	}
}