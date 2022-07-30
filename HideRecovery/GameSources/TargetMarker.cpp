/*!
@file TargetMarker.cpp
@brief TargetMarker�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetMarker.h"

#include "CircleMarkerObject.h"
#include "SegmentMarkerObject.h"
#include "SuccessMarkerObject.h"

#include "PressCenterManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	struct TargetMarker::Parametor
	{
		ex_weak_ptr<GameObject> circleObject;           //CircleUI
		ex_weak_ptr<GameObject> targetSegmentObject;    //�^�[�Q�b�g�����ɂ���SEGMENT
		ex_weak_ptr<GameObject> successSegmentObject;   //Success����Marker
		vector<ex_weak_ptr<GameObject>> segmentObjects; //�ǂɌ�����Segment�̎擾

		int currentUseNum = 0;  //�g�p���̐����擾����B
	};

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�}�[�J�[�{��
	//--------------------------------------------------------------------------------------

	TargetMarker::TargetMarker(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_param(new Parametor())
	{}

	void TargetMarker::OnCreate() {
		//CirlceObject�̐���
		CreateCircleObject();

		//�^�[�Q�b�g�p��Segment����
		std::shared_ptr<GameObject> segment = GetStage()->AddGameObject<SegmentMarkerObject>();
		m_param->targetSegmentObject = segment;

		//Success����Segment����
		std::shared_ptr<GameObject> successSegment = GetStage()->AddGameObject<SuccessMarkerObject>();
		successSegment->SetDrawActive(false);
		m_param->successSegmentObject = successSegment;
	}

	void TargetMarker::CreateCircleObject() {
		auto circle = GetStage()->AddGameObject<CircleMarkerObject>();
		circle->SetDrawActive(false);

		m_param->circleObject = circle;
	}

	std::shared_ptr<GameObject> TargetMarker::AddSegmentObject() {
		std::shared_ptr<GameObject> segmentObject = GetStage()->AddGameObject<SegmentMarkerObject>();
	
		m_param->segmentObjects.push_back(segmentObject);
		return segmentObject;
	}

	void TargetMarker::NotUseSegmentManager(const int& useNum) {
		const auto& markers = m_param->segmentObjects;
		for (int i = useNum; i < markers.size(); i++) {
			markers[i]->SetDrawActive(false);
		}
	}

	void TargetMarker::AcceptUseSegmentMarker(const int& useNum) {
		auto& markers = m_param->segmentObjects;
		for (int i = 0; i < useNum; i++) {
			if (markers.size() > i) { //�}�[�J�[�����݂�����
				markers[i]->SetDrawActive(true);
			}
			else {  //�}�[�J�[�����݂��Ȃ�
				auto marker = AddSegmentObject();
			}
		}

		NotUseSegmentManager(useNum);

		m_param->currentUseNum = useNum; //���ݎg�p���̐����擾
	}

	void TargetMarker::StartSuccess(const std::shared_ptr<GameObject>& segment) {
		segment->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);
		m_param->circleObject->GetComponent<CircleMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);
		m_param->targetSegmentObject->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);

		//Success���g����
		m_param->successSegmentObject->SetDrawActive(true);
	}

	void TargetMarker::EndSuccess() {
		m_param->circleObject->GetComponent<CircleMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);
		m_param->targetSegmentObject->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);

		for (auto& segment : GetSegmentMarkers(GetUseNum())) {
			segment->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);
		}

		//Success���g����
		m_param->successSegmentObject->SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	std::shared_ptr<GameObject> TargetMarker::GetCircleMarker() const {
		return m_param->circleObject.GetShard();
	}

	std::shared_ptr<GameObject> TargetMarker::GetTargetSegmentMarker() const {
		return m_param->targetSegmentObject.GetShard();
	}

	std::shared_ptr<GameObject> TargetMarker::GetSuccessSegmentMarker() const {
		return m_param->successSegmentObject.GetShard();
	}

	vector<std::shared_ptr<GameObject>> TargetMarker::GetSegmentMarkers(const int& getNum) {
		vector<std::shared_ptr<GameObject>> reMarkers;
		auto& markers = m_param->segmentObjects;
		for (int i = 0; i < getNum; i++) {
			if (markers.at(i)) { //�}�[�J�[�����݂�����
				reMarkers.push_back(markers[i].GetShard());
				continue;
			}
			else {  //�}�[�J�[�����݂��Ȃ�
				auto marker = AddSegmentObject();
				reMarkers.push_back(marker);
			}
		}

		return reMarkers;
	}

	vector<ex_weak_ptr<GameObject>> TargetMarker::GetAllSegmentMarkers() const {
		return m_param->segmentObjects;
	}

	void TargetMarker::SetIsUseMarker(const bool isUse) {
		m_param->circleObject->SetDrawActive(isUse);
		m_param->targetSegmentObject->SetDrawActive(isUse);
		AcceptUseSegmentMarker(0);
	}

	int TargetMarker::GetUseNum() const {
		return m_param->currentUseNum;
	}
}

//endbasecross