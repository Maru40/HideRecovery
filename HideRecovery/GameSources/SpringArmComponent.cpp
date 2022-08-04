#include"SpringArmComponent.h"

#include "PlayerInputer.h"
#include "DebugObject.h"

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
			XMConvertToRadians(30.0f),       //Y軸の最大値
			XMConvertToRadians(10.0f),      //Y軸の最小値
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
		lerpSpeed(lerpSpeed)
	{}

	SpringArmComponent::SpringArmComponent(std::shared_ptr<GameObject>& owner) :
		Component(owner), m_armRange(10.0f)
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
	}

	void SpringArmComponent::OnCreate() {
		//m_armRange = 0;
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
		
		m_childTransform->SetPosition(transform->GetPosition() + CalculateDirect() * maxHitData.length);

		//デバッグ
		auto delta = App::GetApp()->GetElapsedTime();
		auto debugSpeed = 3.0f;
		if (PlayerInputer::IsX()) {
			m_armRange += +debugSpeed * delta;
		}

		if (PlayerInputer::IsY()) {
			m_armRange += -debugSpeed * delta;
		}

		if (PlayerInputer::IsXUp() || PlayerInputer::IsYUp()) {
			DebugObject::AddString(L"ArmRange: ", false);
			DebugObject::AddValue(m_armRange);
		}

		if (PlayerInputer::IsBDown()) {
			DebugObject::AddString(L"ArmRange: ", false);
			DebugObject::AddValue(m_armRange);
		}
	}

	Vec3 SpringArmComponent::CalculateDirect() {
		InputYVec();
		InputRXVec();
		return m_param.armVec;
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
}