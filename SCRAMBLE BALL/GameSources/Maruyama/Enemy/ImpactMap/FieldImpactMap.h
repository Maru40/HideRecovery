
/*!
@file FieldImpactMap.h
@brief FieldImpactMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;
	class NavGraphNode;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class ImpactMap;
		class OccupancyManager;

		//--------------------------------------------------------------------------------------
		///	フィールド影響マップのパラメータ
		//--------------------------------------------------------------------------------------
		struct FieldImpactMap_Parametor {
			float intervalRange;        //影響マップノードの間隔
			float createHeightOffset;   //生成する高さのオフセット

			FieldImpactMap_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	フィールド影響マップ
		//--------------------------------------------------------------------------------------
		class FieldImpactMap : public SingletonComponent<FieldImpactMap>
		{
		public:
			using Parametor = FieldImpactMap_Parametor;

		private:
			Parametor m_param; //パラメータ

			std::shared_ptr<ImpactMap> m_impactMap = nullptr;			//影響マップ本体
			std::vector<std::weak_ptr<GameObject>> m_floors;			//影響マップ展開する床データ配列
			//std::vector<OccupancyUpdateData> m_occupancyUpdateDatas;	//占有値更新データ
			std::unique_ptr<OccupancyManager> m_occupancyManager;		//占有値管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="floors">影響マップを展開する床データ配列</param>
			FieldImpactMap(const std::shared_ptr<GameObject>& objPtr, const std::vector<std::shared_ptr<GameObject>>& floors);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="floors">影響マップを展開する床データ配列</param>
			/// <param name="parametor">パラメータ</param>
			FieldImpactMap(const std::shared_ptr<GameObject>& objPtr, const std::vector<std::shared_ptr<GameObject>>& floors, const Parametor& parametor);
			
			void OnLateStart() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// 占有値更新
			/// </summary>
			void OccupancyUpdate();

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

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
			/// <returns></returns>
			std::vector<Vec3> GetRoutePositions(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex);

			/// <summary>
			/// 影響マップデータを取得
			/// </summary>
			/// <returns>影響マップデータ</returns>
			std::shared_ptr<ImpactMap> GetImpactMap() const;

			/// <summary>
			/// 一番近いエリアインデックスを取得する。
			/// </summary>
			/// <param name="position">自分自身の位置</param>
			/// <returns>一番近いエリアインデックス</returns>
			int SearchNearAreaIndex(const Vec3& position) const;

			/// <summary>
			/// ターゲットへのエリア単位の大まかなルートを検索
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="targetPosition">ターゲット位置</param>
			/// <returns>ターゲットへのエリア単位の大まかなルート</returns>
			std::vector<int> SearchAreaIndexRoute(const Vec3& startPosition, const Vec3& targetPosition);

			/// <summary>
			/// ノードの配置間隔距離を取得
			/// </summary>
			/// <returns>ノードの配置間隔距離</returns>
			float GetIntervalRange() const noexcept { return m_param.intervalRange; }

			/// <summary>
			/// 占有値更新するデータを生成
			/// </summary>
			/// <param name="node">更新するノード</param>
			void AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node);

		private:
			/// <summary>
			/// 影響マップのセッティング
			/// </summary>
			/// <param name="objects">影響マップを生成するベースとなる床オブジェクト</param>
			/// <param name="intervalRange">それぞれのノードの間隔距離</param>
			template<class T>
			void SettingImpactMap(const vector<std::weak_ptr<T>>& objects, const float intervalRange) {
				for (const auto& weakObject : objects) {
					constexpr float WallWidth = 1.5f;

					auto object = weakObject.lock();
					auto objectTrans = object->GetComponent<Transform>();

					//位置設定
					auto position = objectTrans->GetPosition();
					position += Vec3::Right()	* WallWidth;
					position += Vec3::Forward() * WallWidth;

					//大きさ設定
					auto scale = objectTrans->GetScale();
					scale.x += -WallWidth;
					scale.z += -WallWidth;

					auto parametor = maru::ImpactMap::Factory_Parametor();
					parametor.rect = maru::Rect(position, scale.x, scale.z);
					parametor.intervalRange = intervalRange;
					parametor.createHeight = m_param.createHeightOffset;
					
					m_impactMap->AddImpactData(parametor);
				}
			}

		};

	}
}