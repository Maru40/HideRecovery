
/*!
@file AIImpacter.cpp
@brief AIImpacter�̃N���X����
�S���F�ێR�T��
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
	///	AI�̉e���҃p�����[�^
	//--------------------------------------------------------------------------------------

	AIImpacter_Parametor::AIImpacter_Parametor() :
		occupancyUpdateIntervalTime(0.5f)
	{}

	//--------------------------------------------------------------------------------------
	///	AI�̉e���Җ{��
	//--------------------------------------------------------------------------------------

	AIImpacter::AIImpacter(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), 
		m_param(Parametor()), 
		m_occupancyTimer(new GameTimer(m_param.occupancyUpdateIntervalTime))
	{}

	void AIImpacter::OnCreate() {
		return;
		//�^�v���X�y�[�X�̃f�o�b�O
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
		if (!m_occupancyTimer->IsTimeUp()) { //���Ԃ��o�߂��Ă��Ȃ��Ȃ�A���������Ȃ�
			return;
		}

		m_occupancyTimer->ResetTimer(m_param.occupancyUpdateIntervalTime);	//�v�����Ԃ����Z�b�g

		auto impacter = GetGameObject()->GetComponent<I_Impacter>(false);	//�e���҂̎擾
		if (!impacter) {
			return;
		}

		maru::ImpactManager impactManager;
		std::shared_ptr<maru::ImpactMap> impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();	//�e���}�b�v�̎擾

		impactManager.CalculateEyeImpact(impactMap, impacter);	//���E�͈̓m�[�h�̍X�V
	}

	//--------------------------------------------------------------------------------------
	///	�C���^�[�t�F�[�X�̎���
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