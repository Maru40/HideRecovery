/*!
@file TargetMarkerManager.cpp
@brief TargetMarkerManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetMarkerManager.h"
#include "TargetMarker.h"

#include "PressData.h"
#include "MaruHitHelper.h"
#include "PressCenterManager.h"
#include "PlayerInputer.h"
#include "LookTargetManager.h"

#include "GameStageBase.h"
#include "ChangeCameraManager.h"

#include "MaruUtility.h"
#include "Mathf.h"

#include "Targeted.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�f�[�^
	//--------------------------------------------------------------------------------------

	struct TargetMarkerManager::TargetData
	{
		ex_weak_ptr<GameObject> self;    //�_�����̃f�[�^
		ex_weak_ptr<GameObject> target;  //�^�[�Q�b�g�f�[�^

		TargetData(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target)
			:self(self), target(target)
		{ }

		Vec3 GetToTargetVec() {
			return target->GetComponent<Transform>()->GetPosition() - self->GetComponent<Transform>()->GetPosition();
		}
	};

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	struct TargetMarkerManager::Parametor
	{
		float circleRadius;  //�T�[�N���̔��a
		Col4 selfSegmentColor = Col4(1.0f, 0.2f, 0.2f, 1.0f); //�������g�Ɍ�����Z�O�����g�̐F
		Col4 segmentColor = Col4(0.2f, 0.2f, 1.0f, 1.0f);     //�ǂɌ������ĕ\������Z�O�����g�̐F
		Col4 successColor = Col4(0.2f, 1.0f, 0.2f, 1.0f);     //�������ɐ؂�ւ��Z�O�����g�̐F

		Parametor()
			:Parametor(1.5f)
		{}

		Parametor(const float& circleRadius)
			:circleRadius(circleRadius)
		{}
	};

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�}�[�J�[�Ǘ�
	//--------------------------------------------------------------------------------------

	TargetMarkerManager::TargetMarkerManager(const std::shared_ptr<GameObject>& objPtr) 
		:Component(objPtr), m_targetDataPtr(new TargetData(nullptr, nullptr)), m_paramPtr(new Parametor()), m_marker(nullptr)
	{}

	void TargetMarkerManager::OnCreate() {
		if (!GetGameStage()) {
			return;
		}

		//�}�[�J�[�̐���
		m_marker = GetStage()->AddGameObject<GameObject>()->AddComponent<TargetMarker>();
	}

	void TargetMarkerManager::OnUpdate() {
		if (HasTarget()) { //�^�[�Q�b�g�����݂���Ȃ�
			MarkerUpdate();
		}
	}

	void TargetMarkerManager::MarkerUpdate() {
		auto pressedManager = GetTarget()->GetComponent<PressCenterManager>(false);
		if (!pressedManager) { //���ꂪ���������珈�������Ȃ��B
			return;
		}

		if(auto targeted = GetTarget()->GetComponent<Targeted>(false)) {
			if (!targeted->GetUpdateActive()) {
				SetTarget(nullptr);
				return;
			}
		}

		CircleMarkerUpdate();
		TargetMarkerUpdate();
		SegmentMarkerUpdate();

		SuccessMarkerUpdate(); //�ׂ��鎞�ɕ\������}�[�J�[
	}

	void TargetMarkerManager::CircleMarkerUpdate() {
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		auto circle = m_marker->GetCircleMarker();
		circle->GetComponent<Transform>()->SetPosition(targetPosition);
	}

	void TargetMarkerManager::TargetMarkerUpdate() {
		auto segment = m_marker->GetTargetSegmentMarker();

		//�}�[�J�[�̃|�W�V������J���[���Z�b�e�B���O
		SettingSegment(segment, -m_targetDataPtr->GetToTargetVec(), m_paramPtr->selfSegmentColor);
	}

	void TargetMarkerManager::SegmentMarkerUpdate() {
		auto pressedManager = GetTarget()->GetComponent<PressCenterManager>(false);
		auto hitDatas = pressedManager->FindHitCandidateHitDatas();  //�q�b�g���f�[�^���擾
		const int useNum = static_cast<int>(hitDatas.size());        //�g�������C���v�b�g
		m_marker->AcceptUseSegmentMarker(useNum);                    //�g������m�点��B
		auto markers = m_marker->GetSegmentMarkers(useNum);          //�g�������擾
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();

		for (int i = 0; i < useNum; i++) {
			auto& data = hitDatas[i];
			
			SettingSegment(markers[i], data.GetHitToSelfVec(), m_paramPtr->segmentColor);
		}
	}

	void TargetMarkerManager::SuccessMarkerUpdate() {
		auto findSegment = FindSuccessSegment();
		if (!findSegment) {
			//SuccessSegment���\���ɂ���B
			m_marker->EndSuccess();
			return;
		}

		m_marker->StartSuccess(findSegment); //�^�[�Q�b�g��Success��ԂɕύX

		//Success�}�[�J�[��Scale�Ȃǂ�ύX
		SettingSuccessSegment();
	}

	void TargetMarkerManager::SettingSegment(const std::shared_ptr<GameObject>& segment, Vec3 direct, const Col4& color) {
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		auto segmentTrans = segment->GetComponent<Transform>();

		//�}�[�J�[�̕������Z�b�g
		segmentTrans->SetForward(direct.GetNormalized());
		//�}�[�J�[�̏ꏊ���Z�b�g
		auto setPosition = targetPosition + (direct.GetNormalized() * m_paramPtr->circleRadius);
		segmentTrans->SetPosition(setPosition);
		//�}�[�J�[�̐F�̃Z�b�g
		auto draw = segment->GetComponent<BcPCTStaticDraw>(false);
		if (draw) {
			draw->SetDiffuse(color);
		}
	}

	void TargetMarkerManager::SettingSuccessSegment() {
		auto segment = m_marker->GetSuccessSegmentMarker();
		auto segmentTrans = segment->GetComponent<Transform>();
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		auto direct = targetPosition - transform->GetPosition();
		auto halfLength = direct.length() * 0.5f;

		auto setPosition = transform->GetPosition() + (direct.GetNormalized() * halfLength);
		segmentTrans->SetForward(direct.GetNormalized());
		segmentTrans->SetPosition(setPosition);
		auto scale = segmentTrans->GetScale();
		scale.z = halfLength;
		segmentTrans->SetScale(scale);
	}

	bool TargetMarkerManager::IsSuccess() const {
		auto targetSegment = m_marker->GetTargetSegmentMarker();
		auto targetForward = targetSegment->GetComponent<Transform>()->GetForward();

		for (auto& segment : m_marker->GetSegmentMarkers(m_marker->GetUseNum())) {
			auto segmentForward = segment->GetComponent<Transform>()->GetForward();
			//�p�x�̍����������Ȃ�
			const float angle = 15.0f;
			if(maru::Mathf::IsInAngleDegree(targetForward, segmentForward, angle)) {
				return true;
			}
		}

		return false;
	}

	std::shared_ptr<GameObject> TargetMarkerManager::FindSuccessSegment() const {
		auto targetSegment = m_marker->GetTargetSegmentMarker();
		auto targetForward = targetSegment->GetComponent<Transform>()->GetForward();

		for (auto& segment : m_marker->GetSegmentMarkers(m_marker->GetUseNum())) {
			auto segmentForward = segment->GetComponent<Transform>()->GetForward();
			//�p�x�̍����������Ȃ�
			const float angle = 15.0f;
			if (maru::Mathf::IsInAngleDegree(targetForward, segmentForward, angle)) {
				return segment;
			}
		}

		return nullptr;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void TargetMarkerManager::SetMarkerActive(const bool isActive) {
		m_marker->SetIsUseMarker(isActive);

		if (HasTarget()) {
			MarkerUpdate();
		}
	}

	bool TargetMarkerManager::HasTarget() const {
		return m_targetDataPtr->target ? true : false;
	}

	void TargetMarkerManager::SetTarget(const std::shared_ptr<GameObject>& target) {
		if (target) {
			SetTargetData(TargetData(GetGameObject(), target));
		}
		else {
			SetTargetData(TargetData(nullptr, nullptr));

			m_marker->EndSuccess();
		}
	}

	void TargetMarkerManager::SetTargetData(const TargetData& data) {
		m_targetDataPtr = std::make_unique<TargetData>(data);
	}

	std::shared_ptr<GameObject> TargetMarkerManager::GetTarget() const {
		return m_targetDataPtr->target.GetShard();
	}

}

//endbasecross