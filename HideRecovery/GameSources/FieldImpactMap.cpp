
/*!
@file FieldImpactMap.cpp
@brief FieldImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "FieldImpactMap.h"

#include "StageMapCSV.h"
#include "ImpactMap.h"

#include "PlayerInputer.h"

namespace basecross {

	namespace maru {

		ex_weak_ptr<FieldImpactMap> SingletonComponent<FieldImpactMap>::sm_instance = nullptr;


		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors
		):
			FieldImpactMap(objPtr, floors, Parametor())
		{}

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors, 
			const Parametor& parametor
		):
			SingletonComponent(objPtr), m_floors(maru::Utility::ConvertArraySharedToWeak(floors)), m_param(parametor)
		{}

		void FieldImpactMap::OnLateStart() {
			m_impactMap = std::make_shared<ImpactMap>(GetStage()); //ImpactMapの生成

			m_impactMap->ClearNodes();  //ノードのクリア
			m_impactMap->ClearEdges();  //エッジのクリア
			const float IntervalRange = m_param.intervalRange; //影響マップの各ノードの距離
			SettingImpactMap(m_floors, IntervalRange);

			m_impactMap->CreateDebugDraw(false); //デバッグデータの生成
		}

		void FieldImpactMap::OnUpdate() {
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsYDown(); });
			}
		}

		std::vector<Vec3> FieldImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			return m_impactMap->GetRoutePositons(selfPosition, targetPosition);
		}

		std::shared_ptr<ImpactMap> FieldImpactMap::GetImpactMap() const {
			return m_impactMap;
		}

	}
}