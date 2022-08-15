
/*!
@file AIImpacter.cpp
@brief AIImpacterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AIImpacter.h"

#include "MaruUtility.h"

#include "PlayerInputer.h"

#include "ImpactMap.h"
#include "SingletonComponent.h"
#include "FieldImpactMap.h"

#include "EyeSearchRange.h"
#include "NavGraphNode.h"

#include "ImpactManager.h"
#include "I_Impact.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "TupleSpace.h"

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