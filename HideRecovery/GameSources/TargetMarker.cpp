/*!
@file TargetMarker.cpp
@brief TargetMarkerのクラス実体
担当：丸山裕喜
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
	/// パラメータ
	//--------------------------------------------------------------------------------------

	struct TargetMarker::Parametor
	{
		ex_weak_ptr<GameObject> circleObject;           //CircleUI
		ex_weak_ptr<GameObject> targetSegmentObject;    //ターゲット方向につかうSEGMENT
		ex_weak_ptr<GameObject> successSegmentObject;   //Success時のMarker
		vector<ex_weak_ptr<GameObject>> segmentObjects; //壁に向かうSegmentの取得

		int currentUseNum = 0;  //使用中の数を取得する。
	};

	//--------------------------------------------------------------------------------------
	/// ターゲットマーカー本体
	//--------------------------------------------------------------------------------------

	TargetMarker::TargetMarker(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_param(new Parametor())
	{}

	void TargetMarker::OnCreate() {
		//CirlceObjectの生成
		CreateCircleObject();

		//ターゲット用のSegment生成
		std::shared_ptr<GameObject> segment = GetStage()->AddGameObject<SegmentMarkerObject>();
		m_param->targetSegmentObject = segment;

		//Success時のSegment生成
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
			if (markers.size() > i) { //マーカーが存在したら
				markers[i]->SetDrawActive(true);
			}
			else {  //マーカーが存在しない
				auto marker = AddSegmentObject();
			}
		}

		NotUseSegmentManager(useNum);

		m_param->currentUseNum = useNum; //現在使用中の数を取得
	}

	void TargetMarker::StartSuccess(const std::shared_ptr<GameObject>& segment) {
		segment->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);
		m_param->circleObject->GetComponent<CircleMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);
		m_param->targetSegmentObject->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Additive);

		//Successを使う時
		m_param->successSegmentObject->SetDrawActive(true);
	}

	void TargetMarker::EndSuccess() {
		m_param->circleObject->GetComponent<CircleMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);
		m_param->targetSegmentObject->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);

		for (auto& segment : GetSegmentMarkers(GetUseNum())) {
			segment->GetComponent<SegmentMarkerObject::DrawComp>()->SetBlendState(BlendState::Opaque);
		}

		//Successを使う時
		m_param->successSegmentObject->SetDrawActive(false);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
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
			if (markers.at(i)) { //マーカーが存在したら
				reMarkers.push_back(markers[i].GetShard());
				continue;
			}
			else {  //マーカーが存在しない
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