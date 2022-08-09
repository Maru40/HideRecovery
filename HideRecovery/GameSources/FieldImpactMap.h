
/*!
@file FieldImpactMap.h
@brief FieldImpactMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class ImpactMap;

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

			std::shared_ptr<ImpactMap> m_impactMap = nullptr; //影響マップ本体
			std::vector<std::weak_ptr<GameObject>> m_floors;  //影響マップ展開する床データ配列

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
			/// <param name="parametor"></param>
			FieldImpactMap(const std::shared_ptr<GameObject>& objPtr, const std::vector<std::shared_ptr<GameObject>>& floors, const Parametor& parametor);
			
			void OnLateStart() override;
			void OnUpdate() override;

		public:

			/// <summary>
			/// 検索したルートを全て取得する。
			/// </summary>
			/// <param name="selfPosition">自分自身のポジション</param>
			/// <param name="targetPosition">目的地</param>
			/// <returns>検索したポジションの配列</returns>
			std::vector<Vec3> GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition);

			/// <summary>
			/// 影響マップデータを取得
			/// </summary>
			/// <returns>影響マップデータ</returns>
			std::shared_ptr<ImpactMap> GetImpactMap() const;

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