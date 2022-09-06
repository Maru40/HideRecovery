
/*!
@file AIImpacter.cpp
@brief AIImpacterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AIImpacter.h"

#include "Maruyama/Utility/Utility.h"

#include "PlayerInputer.h"

#include "ImpactMap.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "FieldImpactMap.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "ImpactManager.h"
#include "Maruyama/Interface/I_Impact.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "../AIDirector/TupleSpace.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	AIの影響者パラメータ
	//--------------------------------------------------------------------------------------

	AIImpacter_Parametor::AIImpacter_Parametor() :
		occupancyUpdateIntervalTime(0.5f)
	{}

	//--------------------------------------------------------------------------------------
	///	AIの影響者本体
	//--------------------------------------------------------------------------------------

	AIImpacter::AIImpacter(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), 
		m_param(Parametor()), 
		m_occupancyTimer(new GameTimer(m_param.occupancyUpdateIntervalTime))
	{}

	void AIImpacter::OnCreate() {
		return;
		//タプルスペースのデバッグ
		using namespace Enemy::Tuple;

		auto tupleSpace = TupleSpace();

		tupleSpace.Write<TupleRequestBase>(GetGameObject(), 1.0f);

		auto isFunc = [](const std::shared_ptr<TupleRequestBase>& request) { return request->value > 0.5f; };
		auto read = tupleSpace.Read<TupleRequestBase>(isFunc);
		auto read2 = tupleSpace.Take(read);

		auto r2 = tupleSpace.Read<TupleRequestBase>(isFunc);
	}

	void AIImpacter::OnUpdate() {
		UpdateEyeRangeImpactMap();
	}

	void AIImpacter::UpdateEyeRangeImpactMap() {
		m_occupancyTimer->UpdateTimer();
		if (!m_occupancyTimer->IsTimeUp()) { //時間が経過していないなら、処理をしない
			return;
		}

		m_occupancyTimer->ResetTimer(m_param.occupancyUpdateIntervalTime);	//計測時間をリセット

		auto impacter = GetGameObject()->GetComponent<I_Impacter>(false);	//影響者の取得
		if (!impacter) {
			return;
		}

		maru::ImpactManager impactManager;
		std::shared_ptr<maru::ImpactMap> impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();	//影響マップの取得

		impactManager.CalculateEyeImpact(impactMap, impacter);	//視界範囲ノードの更新
	}

	//--------------------------------------------------------------------------------------
	///	インターフェースの実装
	//--------------------------------------------------------------------------------------

	std::shared_ptr<GameObject> AIImpacter::GetImpacterObject() const noexcept {
		return GetGameObject();
	}

	std::shared_ptr<EyeSearchRange> AIImpacter::GetEyeSearchRange() const {
		return GetGameObject()->GetComponent<EyeSearchRange>();
	}

	std::shared_ptr<maru::ImpactMap> AIImpacter::GetImpactMap() const {
		return maru::FieldImpactMap::GetInstance()->GetImpactMap();
	}

}

//endbasecross