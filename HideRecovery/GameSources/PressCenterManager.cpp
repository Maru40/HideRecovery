/*!
@file PressCenterManager.cpp
@brief PressCenterManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HitStopManager.h"
#include "PressData.h"
#include "PressCenterManager.h"

#include "PressRefrectionManager.h"
#include "MaruUtility.h"
#include "UtilityObstacle.h"
#include "MaruHitHelper.h"
#include "Mathf.h"
#include "UtilityVelocity.h"
#include <directXmath.h>
#include "Pusher.h"
#include "SlowTimerManager.h"

#include "GameStageBase.h"
#include "TackleCameraObject.h"
#include "MainCameraObject.h"

#include "I_Pressed.h"
#include "GameManagerObject.h"
#include "ChangeCameraManager.h"
#include "VelocityManager.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "CameraShake.h"

#include "ReactiveBool.h"

#include "DebugObject.h"
#include "PlayerInputer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	PressCenterManager::Parametor::Parametor()
		:Parametor(
			3.0f,
			10.0f, 20.0f,
			SlowTimeParametor(1.0f, 0.1f),
			HitStopManager::Data(0.25f, 0.05f)
		)
	{}

	PressCenterManager::Parametor::Parametor(
		const float& searchRange,
		const float& pressedDegree, const float& pressedGoodDegree,
		const SlowTimeParametor& slowParam,
		const HitStopManager::Data& hitStopData,
		const bool isPressed
	) :
		searchRange(searchRange),
		pressedRad(XMConvertToRadians(pressedDegree)), pressedGoodRad(XMConvertToRadians(pressedGoodDegree)),
		slowParam(slowParam),
		hitStopData(hitStopData),
		isPressed(maru::ReactiveBool(isPressed))
	{}

	//--------------------------------------------------------------------------------------
	/// 潰す中心管理本体
	//--------------------------------------------------------------------------------------

	PressCenterManager::PressCenterManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_pusher(nullptr)
	{}

	PressData PressCenterManager::CreatePressData(const CollisionPair& pair, const PressData::MissData& missData, const PressType pressType) {
		if (!m_pusher) {
			return PressData();
		}
		auto pusher = m_pusher->GetGameObject();
		auto pressed = GetGameObject();
		auto obstacle = pair.m_Dest.lock()->GetGameObject();
		PressData data(pusher, pressed, obstacle);
		data.slowParam = m_param.slowParam;
		data.missData = missData;
		data.pressType = pressType;
		data.collisionPair = pair;
		auto velocity = m_pusher->GetGameObject()->GetComponent<VelocityManager>(false);
		if (velocity) {
			data.pressedVec = velocity->GetVelocity();
		}
		return data;
	}

	PressData::MissData PressCenterManager::CreateMissData(const CollisionPair& pair) {
		if (!m_pusher) {
			PressData::MissData missData;
			return missData;
		}
		auto pusherTrans = m_pusher->GetGameObject()->GetComponent<Transform>();
		auto pusherPosition = pusherTrans->GetPosition();

		auto toSelf = transform->GetPosition() - pusherPosition;
		auto power = pusherTrans->GetVelocity().length();
		PressData::MissData missData;
		missData.pusherVec = toSelf.GetNormalized() * power;
		missData.refrectionVec = UtilityVelocity::Reflection(toSelf.GetNormalized() * power, pair.m_SrcHitNormal);

		return missData;
	}

	vector<maru::HitData> PressCenterManager::FindNearObstacleHitDatas() {
		vector<maru::HitData> reDatas;
		auto obstacles = maru::UtilityObstacle::FindObstacles(GetStage()->GetGameObjectVec());

		for (auto& obstacle : obstacles) {
			auto collision = obstacle->GetComponent<CollisionObb>(false);
			if (collision == nullptr) {
				continue;
			}

			auto data = maru::HitHelper::ClosestPtPointOBB(transform->GetPosition(), collision);
			auto toPointVec = data.point - transform->GetPosition();
			if (toPointVec.length() < m_param.searchRange) {  //近かったら
				data.hitObject = obstacle;
				data.selfObject = GetGameObject();
				reDatas.push_back(data);
			}
		}

		return reDatas;
	}

	bool PressCenterManager::IsHitCandidate(const maru::HitData& data) {
		auto collision = GetGameObject()->GetComponent<CollisionObb>(false);
		if (!collision) {
			return false;
		}

		auto obb = collision->GetObb();
		auto halfScale = transform->GetScale() * 0.5f; //半分のスケール

		struct Pair {
			Vec3 start;
			Vec3 end;
			Pair(const Vec3& start, const Vec3& end)
				:start(start), end(end)
			{}
		};
		
		//自分に向かって飛ばす。
		auto start = data.point;  //当たった場所を開始地点に置く。
		auto end = start + data.normal * m_param.searchRange;  //法線方向にSerarRange分伸ばした場所を終了地点にする。

		auto right = maru::Mathf::Mat4RotationAxisY(data.normal, XM_PIDIV2); //Normalの右方向を取得。
		auto rightOffset = right.normalize() * halfScale.x;    //右方向にhalfScale分のoffsetを取得。

		vector<Pair> pairs = {
			Pair(start, end),                              //Center
			Pair(start + rightOffset, end + rightOffset),  //Right
			Pair(start - rightOffset, end - rightOffset)   //Left
		};

		auto hitObb = data.hitObject->GetComponent<CollisionObb>()->GetObb();
		for (auto& pair : pairs) {
			//自分に当たらなかったら
			if (!HitTest::SEGMENT_OBB(pair.start, pair.end, obb)) {
				return false; //即座に当たってない判定
			}

			//壁に当たらなかったら、はみ出しているからfalse
			auto inverseEnd = pair.start + -data.normal * m_param.searchRange;
			if (!HitTest::SEGMENT_OBB(pair.start, inverseEnd, hitObb)) {
				return false;
			}
		}

		//全てヒットしたからture
		return true;
	}

	bool PressCenterManager::IsHitObject(const std::shared_ptr<GameObject>& object) {
		auto objs = FindHitCandidateHitDatas();  //壁の候補を取得
		for (auto obj : objs) {
			if (obj.hitObject.GetShard() == object) { //同じなら
				return true;
			}
		}

		return false;
	}

	bool PressCenterManager::IsPusherAngle(const PressType type) {
		return (type == PressType::Miss) ? false : true;
	}

	PressType PressCenterManager::CalculatePressType(const CollisionPair& pair) const {
		if (!m_pusher) {
			return PressType::Miss;
		}
		auto object = m_pusher->GetGameObject();
		auto pusherTrans = object->GetComponent<Transform>();
		auto pusherVelocity = pusherTrans->GetVelocity();
		auto normal = pair.m_SrcHitNormal;
		auto pusherHitData = maru::HitHelper::ClosestPtPointOBB(pair.m_CalcHitPoint, m_pusher->GetGameObject()->GetComponent<CollisionObb>());

		auto newDot = dot(-pusherHitData.normal.GetNormalized(), normal.GetNormalized());
		auto angle = acosf(newDot);

		if (angle < m_param.pressedRad) {
			return PressType::Perfect;
		}

		if (angle < m_param.pressedGoodRad) {
			return PressType::Good;
		}

		return PressType::Miss;
	}

	void PressCenterManager::StartCameraShake(const float& time, const maru::DeltaType& deltaType) {
		GetGameStage()->GetMainCameraObject()->GetComponent<CameraShake>()->StartShake(time, maru::DeltaType::Real);
		GetGameStage()->GetTackleCameraObject()->GetComponent<CameraShake>()->StartShake(time, maru::DeltaType::Real);
	}

	void PressCenterManager::SuccessPressProcess(const CollisionPair& pair, const PressType pressType) {
		DebugObject::AddString(L"SuccessPressProcess");

		//潰される処理
		auto pressData = CreatePressData(pair, PressData::MissData(), pressType);
		SetSuccessPressData(pressData);  //潰し成功データを保存
		m_pusher->SuccessPressing(pressData);
	}

	void PressCenterManager::MissPressProcess(const CollisionPair& pair, const PressType pressType) {
		auto refrectionManager = GetGameObject()->GetComponent<PressRefrectionManager>(false);
		if (refrectionManager == nullptr) {
			DebugObject::AddString(L"PressCenterManager::MissPressProcess: PressRefrectionManagerが存在しません。");
			return;
		}

		auto missData = CreateMissData(pair);
		refrectionManager->StartReflection(CreatePressData(pair, missData, pressType));

		if (m_pusher) {
			m_pusher->MissPressing(pair);
		}
	}

	void PressCenterManager::SlimeHit(const CollisionPair& pair) {
		if (!IsPressed()) { //Pressed状態出ないなら。
			return; //処理を飛ばす。
		}

		auto other = pair.m_Dest.lock();
		auto pressCenterManager = other->GetGameObject()->GetComponent<PressCenterManager>(false);

		//PressManagerをアタッチされている。
		if (pressCenterManager) {
			m_pusher->StartPush(pair);
		}
	}

	void PressCenterManager::PressObstacleHit(const CollisionPair& pair) {
		if (IsPressed()) {  //プレス状態なら
			auto other = pair.m_Dest.lock();
			if (!maru::UtilityObstacle::IsObstacle(other->GetGameObject())) {  //障害物でないなら処理をしない
				return;
			}
			
			auto pressType = CalculatePressType(pair);
			//Pusherが垂直に当たったかどうか、&&、判定の取れる障害物
			if (IsPusherAngle(pressType) && IsHitObject(other->GetGameObject())) {
				SuccessPressProcess(pair, pressType);
				return;
			}

			MissPressProcess(pair, pressType);  //失敗プロセス。
		}
	}

 	void PressCenterManager::OnCollisionEnter(const CollisionPair& pair) {
		PressObstacleHit(pair);  //障害物に当たった時に判定を取って、成功したら潰す。
		SlimeHit(pair);
	}

	void PressCenterManager::OnCollisionExcute(const CollisionPair& pair) {
		PressObstacleHit(pair);
		SlimeHit(pair);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	vector<maru::HitData> PressCenterManager::FindHitCandidateHitDatas() {
		vector<maru::HitData> reObjs;
		auto datas = FindNearObstacleHitDatas();

		for (auto& data : datas) {

			if (IsHitCandidate(data)) { //ヒット候補に含まれるのなら
				reObjs.push_back(data);
			}
		}

		return reObjs;
	}

	void PressCenterManager::SetIsPressed(const bool isPressed) noexcept {
		m_param.isPressed = isPressed;
	}

	bool PressCenterManager::IsPressed() const {
		if (!m_pusher) {
			return false;
		}

		return m_param.isPressed.GetValue();
	}

	void PressCenterManager::AddIsPressedReactiveFunction(const bool b, const std::function<void()>& function) {
		m_param.isPressed.AddFunction(b, function);
	}

	void PressCenterManager::SetSuccessPressData(const PressData& data) noexcept {
		m_param.successPressData = data;
	}

	PressData PressCenterManager::GetSuccessPressData() const noexcept {
		return m_param.successPressData;
	}

}

//endbasecross