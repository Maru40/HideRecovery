/*!
@file PressCenterManager.cpp
@brief PressCenterManager�̃N���X����
�S���F�ێR�T��
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
	/// �p�����[�^
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
	/// �ׂ����S�Ǘ��{��
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
			if (toPointVec.length() < m_param.searchRange) {  //�߂�������
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
		auto halfScale = transform->GetScale() * 0.5f; //�����̃X�P�[��

		struct Pair {
			Vec3 start;
			Vec3 end;
			Pair(const Vec3& start, const Vec3& end)
				:start(start), end(end)
			{}
		};
		
		//�����Ɍ������Ĕ�΂��B
		auto start = data.point;  //���������ꏊ���J�n�n�_�ɒu���B
		auto end = start + data.normal * m_param.searchRange;  //�@��������SerarRange���L�΂����ꏊ���I���n�_�ɂ���B

		auto right = maru::Mathf::Mat4RotationAxisY(data.normal, XM_PIDIV2); //Normal�̉E�������擾�B
		auto rightOffset = right.normalize() * halfScale.x;    //�E������halfScale����offset���擾�B

		vector<Pair> pairs = {
			Pair(start, end),                              //Center
			Pair(start + rightOffset, end + rightOffset),  //Right
			Pair(start - rightOffset, end - rightOffset)   //Left
		};

		auto hitObb = data.hitObject->GetComponent<CollisionObb>()->GetObb();
		for (auto& pair : pairs) {
			//�����ɓ�����Ȃ�������
			if (!HitTest::SEGMENT_OBB(pair.start, pair.end, obb)) {
				return false; //�����ɓ������ĂȂ�����
			}

			//�ǂɓ�����Ȃ�������A�͂ݏo���Ă��邩��false
			auto inverseEnd = pair.start + -data.normal * m_param.searchRange;
			if (!HitTest::SEGMENT_OBB(pair.start, inverseEnd, hitObb)) {
				return false;
			}
		}

		//�S�ăq�b�g��������ture
		return true;
	}

	bool PressCenterManager::IsHitObject(const std::shared_ptr<GameObject>& object) {
		auto objs = FindHitCandidateHitDatas();  //�ǂ̌����擾
		for (auto obj : objs) {
			if (obj.hitObject.GetShard() == object) { //�����Ȃ�
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

		//�ׂ���鏈��
		auto pressData = CreatePressData(pair, PressData::MissData(), pressType);
		SetSuccessPressData(pressData);  //�ׂ������f�[�^��ۑ�
		m_pusher->SuccessPressing(pressData);
	}

	void PressCenterManager::MissPressProcess(const CollisionPair& pair, const PressType pressType) {
		auto refrectionManager = GetGameObject()->GetComponent<PressRefrectionManager>(false);
		if (refrectionManager == nullptr) {
			DebugObject::AddString(L"PressCenterManager::MissPressProcess: PressRefrectionManager�����݂��܂���B");
			return;
		}

		auto missData = CreateMissData(pair);
		refrectionManager->StartReflection(CreatePressData(pair, missData, pressType));

		if (m_pusher) {
			m_pusher->MissPressing(pair);
		}
	}

	void PressCenterManager::SlimeHit(const CollisionPair& pair) {
		if (!IsPressed()) { //Pressed��ԏo�Ȃ��Ȃ�B
			return; //�������΂��B
		}

		auto other = pair.m_Dest.lock();
		auto pressCenterManager = other->GetGameObject()->GetComponent<PressCenterManager>(false);

		//PressManager���A�^�b�`����Ă���B
		if (pressCenterManager) {
			m_pusher->StartPush(pair);
		}
	}

	void PressCenterManager::PressObstacleHit(const CollisionPair& pair) {
		if (IsPressed()) {  //�v���X��ԂȂ�
			auto other = pair.m_Dest.lock();
			if (!maru::UtilityObstacle::IsObstacle(other->GetGameObject())) {  //��Q���łȂ��Ȃ珈�������Ȃ�
				return;
			}
			
			auto pressType = CalculatePressType(pair);
			//Pusher�������ɓ����������ǂ����A&&�A����̎����Q��
			if (IsPusherAngle(pressType) && IsHitObject(other->GetGameObject())) {
				SuccessPressProcess(pair, pressType);
				return;
			}

			MissPressProcess(pair, pressType);  //���s�v���Z�X�B
		}
	}

 	void PressCenterManager::OnCollisionEnter(const CollisionPair& pair) {
		PressObstacleHit(pair);  //��Q���ɓ����������ɔ��������āA����������ׂ��B
		SlimeHit(pair);
	}

	void PressCenterManager::OnCollisionExcute(const CollisionPair& pair) {
		PressObstacleHit(pair);
		SlimeHit(pair);
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	vector<maru::HitData> PressCenterManager::FindHitCandidateHitDatas() {
		vector<maru::HitData> reObjs;
		auto datas = FindNearObstacleHitDatas();

		for (auto& data : datas) {

			if (IsHitCandidate(data)) { //�q�b�g���Ɋ܂܂��̂Ȃ�
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