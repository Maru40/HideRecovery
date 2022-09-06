#include"SpringArmComponent.h"

#include "PlayerInputer.h"
#include "Maruyama/Debug/Object/DebugObject.h"

#include "Maruyama/Camera/Component/LookAtCameraManager.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"
#include "Maruyama/Player/Component/Teleport.h"

#include "StageMapCSV.h"

namespace basecross
{
	SpringArmComponent::Parametor::Parametor()
		:Parametor(
			20.0f,                           //アームレンジ
			Vec3(0.0f, 0.5f, 0.0f),          //アームの初期方向
			XMConvertToRadians(24.0f),       //初期回転
			XMConvertToRadians(-90.0f),      //初期回転位置
			3.0f,                            //Y軸回転スピード
			3.0f,                            //Z軸回転スピード
			XMConvertToRadians(17.0f),       //Y軸の最大値
			XMConvertToRadians(17.0f),       //Y軸の最小値
			100.0f)                          //追従スピード
	{}

	SpringArmComponent::Parametor::Parametor(
		const float& armRange,
		const Vec3& armVec,
		const float& radY,
		const float& radXZ,
		const float& speedY,
		const float& speedXZ,
		const float& maxY,
		const float& minY,
		const float& lerpSpeed
	) :
		armRange(armRange),
		armVec(armVec),
		radY(radY),
		radXZ(radXZ),
		speedY(speedY),
		speedXZ(speedXZ),
		maxY(maxY),
		minY(minY),
		lerpSpeed(lerpSpeed),
		armSpeed(25.0f),
		currentArmRange(10.0f)
	{}

	SpringArmComponent::SpringArmComponent(std::shared_ptr<GameObject>& owner) :
		Component(owner), m_armRange(6.0f)
	{

	}

	void SpringArmComponent::SetArmRange(const float armRange)
	{
		m_armRange = armRange;
	}

	float SpringArmComponent::GetArmRange() const
	{
		return m_armRange;
	}

	void SpringArmComponent::AddHitTag(const std::wstring& hitTag)
	{
		m_hitTags.push_back(hitTag);
	}

	void SpringArmComponent::RemoveHitTag(const std::wstring& hitTag)
	{
		auto it = std::find(m_hitTags.begin(), m_hitTags.end(), hitTag);

		if (it != m_hitTags.end())
		{
			m_hitTags.erase(it);
		}
	}

	void SpringArmComponent::SetChildObject(const std::shared_ptr<GameObject>& childObject)
	{
		m_childObject = childObject;
		m_childTransform = childObject->GetComponent<Transform>();

		m_direction = m_childTransform->GetPosition();
		m_direction.normalize();

		auto lookAt = m_childObject->GetComponent<LookAtCameraManager>(false);
		if (lookAt) {
			m_defaultCenterOffset = lookAt->GetParametor().centerOffset;
		}
	}

	void SpringArmComponent::OnCreate() {
		CheckRimitY();  //角度制限
		m_param.armVec.y = sinf(m_param.radY);
		m_param.armVec.x = cosf(m_param.radXZ);
		m_param.armVec.z = sinf(m_param.radXZ);
		m_param.currentArmRange = m_armRange;

		constexpr int speedIndex = 9;
		auto speedStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", speedIndex);
		auto speed = static_cast<float>(_wtof(speedStr.c_str()));
		m_param.speedXZ = speed;
	}

	void SpringArmComponent::OnUpdate2()
	{
		if (!m_childObject)
		{
			return;
		}

		auto startPosition = transform->GetWorldPosition();
		auto pos = m_childTransform->GetWorldPosition();
		auto rayDirection = m_childTransform->GetWorldPosition() - startPosition;

		rayDirection.normalize();

		RayHitData maxHitData;
		maxHitData.length = m_armRange;
		maxHitData.point = startPosition + rayDirection * m_armRange;

		for (auto& object : GetStage()->GetGameObjectVec())
		{
			auto collision = object->GetComponent<Collision>(false);

			if (!collision)
			{
				continue;
			}

			bool isHitTagFind = false;

			for (auto& hitTag : m_hitTags)
			{
				if (object->FindTag(hitTag))
				{
					isHitTagFind = true;
					break;
				}
			}

			if (!isHitTagFind)
			{
				continue;
			}

			RayHitData hitData;
			bool isHit = collision->IsRayHit(startPosition, rayDirection, hitData);

			if (!isHit)
			{
				continue;
			}

			if (hitData.length < maxHitData.length)
			{
				maxHitData = hitData;
			}
		}
		
		//m_childTransform->SetPosition(transform->GetPosition() + CalculateDirect() * maxHitData.length);
		//m_childTransform->SetPosition(CalculatePosition(maxHitData.length));
		auto armRange = CalculateArmRange(maxHitData.length);
		m_childTransform->SetPosition(transform->GetPosition() + CalculateDirect() * armRange);
		CheckLookAt(armRange);
	}

	Vec3 SpringArmComponent::CalculateDirect() {
		if (FieldMap::GetInstance()->GetMapDraw()) {
			return m_param.armVec;
		}

		auto player = m_player.lock();
		if (player) {
			auto teleport = player->GetComponent<Teleport>(false);
			if (teleport && teleport->IsTeleporting()) {
				return m_param.armVec;
			}
		}

		InputYVec();
		InputRXVec();
		return m_param.armVec;
	}

	Vec3 SpringArmComponent::CalculatePosition(const float& targetLength) {
		constexpr float NearRange = 0.25f;
		auto targetPosition = transform->GetPosition() + CalculateDirect() * targetLength;	//目的地
		auto toTargetPosition = targetPosition - m_childTransform->GetPosition();
		if (toTargetPosition.length() < NearRange) {  //近い位置にいたら。
			return m_childTransform->GetPosition();
		}

		auto delta = App::GetApp()->GetElapsedTime();
		auto setPosition = m_childTransform->GetPosition() + toTargetPosition.GetNormalized() * m_param.armSpeed * delta;
		return setPosition;
	}

	float SpringArmComponent::CalculateArmRange(const float& targetRange) {
		if (targetRange == m_param.currentArmRange) {
			return targetRange;
		}

		auto delta = App::GetApp()->GetElapsedTime();
		float range = Lerp::CalculateLerp(m_param.currentArmRange, targetRange, 0, 1, m_param.armSpeed * delta, Lerp::rate::Linear);
		m_param.currentArmRange = range;

		return range;
	}

	void SpringArmComponent::InputYVec()   //y軸関係の処理
	{
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		auto pad = device.GetControlerVec()[0];
		auto delta = app->GetElapsedTime();

		if (!pad.bConnected)
		{
			return;
		}

		float vertical = pad.fThumbRY;

		if (vertical != 0.0f)
		{
			m_param.radY += -vertical * delta * m_param.speedY;

			CheckRimitY();  //角度制限

			m_param.armVec.y = sinf(m_param.radY);
		}
	}

	void SpringArmComponent::InputRXVec()    //x軸関係の処理
	{
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		auto pad = device.GetControlerVec()[0];
		auto delta = app->GetElapsedTime();

		if (!pad.bConnected)
		{
			return;
		}

		float horizontal = pad.fThumbRX;

		if (horizontal != 0)
		{
			m_param.radXZ += -horizontal * delta * m_param.speedXZ;

			m_param.armVec.x = cosf(m_param.radXZ);
			m_param.armVec.z = sinf(m_param.radXZ);
		}

		if (abs(m_param.radXZ) >= XM_2PI) {
			//一回転したら0に戻す
			m_param.radXZ = 0.0f;
		}
	}


	void SpringArmComponent::CheckRimitY()
	{
		if (m_param.radY >= m_param.maxY)  //上限を超えたら
		{
			m_param.radY = m_param.maxY;
		}

		if (m_param.radY <= m_param.minY)  //下限を超えたら
		{
			m_param.radY = m_param.minY;
		}
	}

	void SpringArmComponent::CheckLookAt(const float armRange) {
		if (!m_childObject) {
			return;
		}

		constexpr float MaxSub = 0.95f;
		auto subRange = m_armRange - armRange;
		auto rate = subRange / m_armRange;	//短くなるほど高くなる。

		auto lookAt = m_childObject->GetComponent<LookAtCameraManager>(false);
		if (!lookAt) {
			return;
		}

		if (m_defaultCenterOffset == Vec3(0)) {
			m_defaultCenterOffset = lookAt->GetParametor().centerOffset;
		}

		if (m_armRange == armRange) {
			return;
		}

		auto sub = MaxSub * rate;
		auto param = lookAt->GetParametor();
		param.centerOffset.y = m_defaultCenterOffset.y - sub;
		lookAt->SetParametor(param);

		auto player = m_player.lock();
		if (!player) {
			return;
		}

		auto teleport = player->GetComponent<Teleport>(false);
		auto draw = player->GetComponent<PlayerObject::DrawComp>(false);
		if (draw && teleport) {
			auto diffuse = draw->GetDiffuse();
			float diffuseRate = (armRange / m_armRange);

			//完全に消す
			constexpr float VanishRate = 0.2f;
			if (diffuseRate < VanishRate) {
				if (!teleport->IsTeleporting()) {
					draw->SetDrawActive(false);
				}
			}
			else {
				if (!teleport->IsTeleporting()) {
					draw->SetDrawActive(true);
				}
			}

			//薄くする
			if (diffuseRate < 0.75f) {
				diffuse.w = 1.0f * diffuseRate;
				draw->SetDiffuse(diffuse);
			}
			else {
				diffuse.w = 1.0f;
				draw->SetDiffuse(diffuse);
			}
		}
	}
}