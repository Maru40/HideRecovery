
/*!
@file CellMapImpactController.h
@brief CellMapImpactControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Thread/FutureData.h"

#include "Maruyama/Interface/I_Impact.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class ThreadPool;

	namespace maru {
	
		class Cell;
		class CellMap;

		//--------------------------------------------------------------------------------------
		///	セルマップの影響度コントローラー
		//--------------------------------------------------------------------------------------
		class CellMapImpactController
		{
		public:
			using Cells = std::vector<std::shared_ptr<Cell>>;
			using FutureData = FutureData<Cells>;

		private:
			mutable std::mutex m_mutex;					//ミューテックス
			std::unique_ptr<ThreadPool> m_threadPool;	//スレッドプール

			std::weak_ptr<CellMap> m_cellMap;			//セルマップ

		public:
			CellMapImpactController(
				const std::shared_ptr<CellMap>& cellMap,
				const std::uint_fast32_t& thread_count = std::thread::hardware_concurrency()
			);

			virtual ~CellMapImpactController() = default;

		private:
			/// <summary>
			/// 影響度の更新
			/// </summary>
			/// <param name="futureData">フューチャーデータ</param>
			/// <returns>更新されたデータ</returns>
			Cells ThreadWoker_CalculateImpactCells(
				const std::weak_ptr<GameObject>& requester, 
				const std::shared_ptr<FutureData>& futureData
			);

		public:

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// タスクのお願い
			/// </summary>
			/// <param name="requester">お願いした人</param>
			std::shared_ptr<FutureData> AddTask(const std::weak_ptr<GameObject>& requester);
		};

	}
}