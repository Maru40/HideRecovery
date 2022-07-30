/*!
@file TargetMarkerManager.cpp
@brief TargetMarkerManagerのクラス実体
担当：丸山裕喜
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
	/// ターゲットデータ
	//--------------------------------------------------------------------------------------

	struct TargetMarkerManager::TargetData
	{
		ex_weak_ptr<GameObject> self;    //狙う側のデータ
		ex_weak_ptr<GameObject> target;  //ターゲットデータ

		TargetData(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target)
			:self(self), target(target)
		{ }

		Vec3 GetToTargetVec() {
			return target->GetComponent<Transform>()->GetPosition() - self->GetComponent<Transform>()->GetPosition();
		}
	};

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	struct TargetMarkerManager::Parametor
	{
		float circleRadius;  //サークルの半径
		Col4 selfSegmentColor = Col4(1.0f, 0.2f, 0.2f, 1.0f); //自分自身に向けるセグメントの色
		Col4 segmentColor = Col4(0.2f, 0.2f, 1.0f, 1.0f);     //壁に向かって表示するセグメントの色
		Col4 successColor = Col4(0.2f, 1.0f, 0.2f, 1.0f);     //成功時に切り替わるセグメントの色

		Parametor()
			:Parametor(1.5f)
		{}

		Parametor(const float& circleRadius)
			:circleRadius(circleRadius)
		{}
	};

	//--------------------------------------------------------------------------------------
	/// ターゲットマーカー管理
	//--------------------------------------------------------------------------------------

	TargetMarkerManager::TargetMarkerManager(const std::shared_ptr<GameObject>& objPtr) 
		:Component(objPtr), m_targetDataPtr(new TargetData(nullptr, nullptr)), m_paramPtr(new Parametor()), m_marker(nullptr)
	{}

	void TargetMarkerManager::OnCreate() {
		if (!GetGameStage()) {
			return;
		}

		//マーカーの生成
		m_marker = GetStage()->AddGameObject<GameObject>()->AddComponent<TargetMarker>();
	}

	void TargetMarkerManager::OnUpdate() {
		if (HasTarget()) { //ターゲットが存在するなら
			MarkerUpdate();
		}
	}

	void TargetMarkerManager::MarkerUpdate() {
		auto pressedManager = GetTarget()->GetComponent<PressCenterManager>(false);
		if (!pressedManager) { //これが無かったら処理をしない。
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

		SuccessMarkerUpdate(); //潰せる時に表示するマーカー
	}

	void TargetMarkerManager::CircleMarkerUpdate() {
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		auto circle = m_marker->GetCircleMarker();
		circle->GetComponent<Transform>()->SetPosition(targetPosition);
	}

	void TargetMarkerManager::TargetMarkerUpdate() {
		auto segment = m_marker->GetTargetSegmentMarker();

		//マーカーのポジションやカラーをセッティング
		SettingSegment(segment, -m_targetDataPtr->GetToTargetVec(), m_paramPtr->selfSegmentColor);
	}

	void TargetMarkerManager::SegmentMarkerUpdate() {
		auto pressedManager = GetTarget()->GetComponent<PressCenterManager>(false);
		auto hitDatas = pressedManager->FindHitCandidateHitDatas();  //ヒット候補データを取得
		const int useNum = static_cast<int>(hitDatas.size());        //使う数をインプット
		m_marker->AcceptUseSegmentMarker(useNum);                    //使う数を知らせる。
		auto markers = m_marker->GetSegmentMarkers(useNum);          //使う分を取得
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();

		for (int i = 0; i < useNum; i++) {
			auto& data = hitDatas[i];
			
			SettingSegment(markers[i], data.GetHitToSelfVec(), m_paramPtr->segmentColor);
		}
	}

	void TargetMarkerManager::SuccessMarkerUpdate() {
		auto findSegment = FindSuccessSegment();
		if (!findSegment) {
			//SuccessSegmentを非表示にする。
			m_marker->EndSuccess();
			return;
		}

		m_marker->StartSuccess(findSegment); //ターゲットをSuccess状態に変更

		//SuccessマーカーのScaleなどを変更
		SettingSuccessSegment();
	}

	void TargetMarkerManager::SettingSegment(const std::shared_ptr<GameObject>& segment, Vec3 direct, const Col4& color) {
		auto targetPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		auto segmentTrans = segment->GetComponent<Transform>();

		//マーカーの方向をセット
		segmentTrans->SetForward(direct.GetNormalized());
		//マーカーの場所をセット
		auto setPosition = targetPosition + (direct.GetNormalized() * m_paramPtr->circleRadius);
		segmentTrans->SetPosition(setPosition);
		//マーカーの色のセット
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
			//角度の差が小さいなら
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
			//角度の差が小さいなら
			const float angle = 15.0f;
			if (maru::Mathf::IsInAngleDegree(targetForward, segmentForward, angle)) {
				return segment;
			}
		}

		return nullptr;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
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