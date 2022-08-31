
/*!
@file OccupancyManager.h
@brief OccupancyManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "I_Impact.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class GameTimer;

	namespace maru {

		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	影響データの占有値管理パラメータ
		//--------------------------------------------------------------------------------------
		struct OccupancyManager_Parametor {

		};

		//--------------------------------------------------------------------------------------
		///	占有値更新データ
		//--------------------------------------------------------------------------------------
		struct OccupancyUpdateData {
			float startValue;					//データ生成時の占有値
			std::unique_ptr<GameTimer> timer;	//タイマー
			std::weak_ptr<NavGraphNode> node;	//ノード

			OccupancyUpdateData();

			OccupancyUpdateData(const float intervalTime, const std::shared_ptr<NavGraphNode>& node);

			OccupancyUpdateData(const GameTimer& timer, const std::shared_ptr<NavGraphNode>& node);

			bool operator== (const OccupancyUpdateData& data);
		};

		//--------------------------------------------------------------------------------------
		///	影響データの占有値管理
		//--------------------------------------------------------------------------------------
		class OccupancyManager
		{
		public:
			using UpdateData = OccupancyUpdateData;

		private:
			std::weak_ptr<maru::ImpactMap> m_impactMap;	//影響マップ

			std::vector<UpdateData> m_updateDatas;		//占有値更新データ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="impactMap">影響マップ</param>
			OccupancyManager(const std::shared_ptr<maru::ImpactMap>& impactMap);

			/// <summary>
			/// 更新処理
			/// </summary>
			void OnUpdate();

		private:
			/// <summary>
			/// 占有値更新
			/// </summary>
			void OccupancyValueUpdate();

			/// <summary>
			/// 占有値更新時間を計算して返す。
			/// </summary>
			/// <param name="node">更新するノード</param>
			float CalculateOccupancyIntervalTime(const std::shared_ptr<NavGraphNode>& node);

			/// <summary>
			/// データ配列から削除する
			/// </summary>
			/// <param name="removeDatas">削除したいデータ配列</param>
			void RemoveUpdateDatas(const std::vector<UpdateData*>& removeDatas);

		public:
			/// <summary>
			/// 占有値更新するデータを生成
			/// </summary>
			/// <param name="node">更新するノード</param>
			void AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node);

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 影響マップの取得
			/// </summary>
			/// <returns>影響マップ</returns>
			std::shared_ptr<ImpactMap> GetImpactMap() const { return m_impactMap.lock(); }

		};

	}
}