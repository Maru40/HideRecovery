
/*!
@file OccupancyManager.cpp
@brief OccupancyManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "OccupancyManager.h"

#include "NavGraphNode.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "ImpactMap.h"

namespace basecross {

	namespace maru {
		constexpr float UPDATE_DESIRED_VALUE = 0.5f;	//更新中の目標値

		//--------------------------------------------------------------------------------------
		///	占有値更新データ
		//--------------------------------------------------------------------------------------

		OccupancyUpdateData::OccupancyUpdateData() :
			OccupancyUpdateData(0.0f, nullptr)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const float intervalTime, const std::shared_ptr<NavGraphNode>& node) :
			OccupancyUpdateData(GameTimer(intervalTime), node)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const GameTimer& timer, const std::shared_ptr<NavGraphNode>& node) :
			startValue(node->GetImpactData().occupancyValue),
			timer(new GameTimer(timer)),
			node(node)
		{}

		bool OccupancyUpdateData::operator== (const OccupancyUpdateData& data) {
			return this == &data;
		}

		//--------------------------------------------------------------------------------------
		///	影響データの占有値管理の本体
		//--------------------------------------------------------------------------------------

		OccupancyManager::OccupancyManager(const std::shared_ptr<maru::ImpactMap>& impactMap):
			m_impactMap(impactMap)
		{}

		void OccupancyManager::OnUpdate() {
			OccupancyValueUpdate();
		}

		void OccupancyManager::OccupancyValueUpdate() {
			std::vector<UpdateData*> removeDatas;	//削除するデータをまとめる配列。

			for (auto& data : m_updateDatas) {
				//ノードがnullptrなら、削除対象にして飛ばす。
				auto node = data.node.lock();
				if (!node) {
					removeDatas.push_back(&data);
					continue;
				}

				data.timer->UpdateTimer();	//タイマー更新

				//データの用意
				auto impactData = node->GetImpactData();
				auto timeRate = data.timer->GetTimeRate();

				//占有値の計算
				auto endTime = data.timer->GetIntervalTime();
				auto nowTime = data.timer->GetElapsedTime();
				auto value = Lerp::CalculateLerp(data.startValue, UPDATE_DESIRED_VALUE, 0.0f, endTime, nowTime, Lerp::rate::Linear);

				//データの更新
				impactData.occupancyValue = value;
				node->SetImpactData(impactData);

				//時間が経過したら
				if (data.timer->IsTimeUp()) {
					impactData.occupancyValue = UPDATE_DESIRED_VALUE;	//目標値に設定
					node->SetImpactData(impactData);
					removeDatas.push_back(&data);	//配列から削除する候補に追加
				}

				//デバッグ
				auto colorValue = 1.0f - impactData.occupancyValue;
				GetImpactMap()->ChangeTargetNodeColor(node->GetIndex(), Col4(1.0f, colorValue, colorValue, 1.0f));
			}

			//データの削除
			RemoveUpdateDatas(removeDatas);
		}

		float OccupancyManager::CalculateOccupancyIntervalTime(const std::shared_ptr<NavGraphNode>& node) {
			auto impactData = node->GetImpactData();
			float subDesired = std::abs(UPDATE_DESIRED_VALUE - impactData.occupancyValue);	//目標値との差分値
			float rate = subDesired / UPDATE_DESIRED_VALUE;

			auto intervalTime = impactData.occupacyRecoverySpeed * rate;

			return intervalTime;
		}

		void OccupancyManager::RemoveUpdateDatas(const std::vector<UpdateData*>& removeDatas) {
			//いらないデータを削除
			for (int i = 0; i < removeDatas.size(); i++) {
				auto iter = m_updateDatas.begin();

				while (iter != m_updateDatas.end()) {
					if (&(*iter) == removeDatas[i]) {
						iter = m_updateDatas.erase(iter);
						break;
					}
					iter++;
				}
			}
		}

		void OccupancyManager::AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node) {
			//同じデータが存在するなら、時間の更新のみ
			for (auto& data : m_updateDatas) {
				if (data.node.lock() == node) {
					auto impactData = node->GetImpactData();
					data.timer->ResetTimer(CalculateOccupancyIntervalTime(node));
					return;
				}
			}

			auto impactData = node->GetImpactData();
			m_updateDatas.push_back(OccupancyUpdateData(GameTimer(CalculateOccupancyIntervalTime(node)), node));
		}

	}
}