/*!
@file SeekTPS.cpp
@brief SeekTPSなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "SeekTPS.h"

namespace basecross {

	//パラメータ----------------------------------------------------------------------------------

	SeekTPS::Parametor::Parametor()
		:Parametor(
			20.0f,                           //アームレンジ
			Vec3(0.0f, 0.5f, 0.0f),          //アームの初期方向
			XMConvertToRadians(24.0f),       //初期回転
			XMConvertToRadians(-90.0f),      //初期回転位置
			0.0f,                            //Y軸回転スピード
			3.0f,                            //Z軸回転スピード
			XMConvertToRadians(70.0f),       //Y軸の最大値
			XMConvertToRadians(-20.0f),      //Y軸の最小値
			100.0f,                          //追従スピード
			SeekType::Lerp)                  //追従タイプ
	{}

	SeekTPS::Parametor::Parametor(
		const float& armRange,
		const Vec3& armVec,
		const float& radY,
		const float& radXZ,
		const float& speedY,
		const float& speedXZ,
		const float& maxY,
		const float& minY,
		const float& lerpSpeed,
		const SeekType& seekType
	):
		armRange(armRange),
		armVec(armVec),
		radY(radY),
		radXZ(radXZ),
		speedY(speedY),
		speedXZ(speedXZ),
		maxY(maxY),
		minY(minY),
		lerpSpeed(lerpSpeed),
		seekType(seekType)
	{}

	//--------------------------------------------------------------------------------------------

	void SeekTPS::OnCreate()
	{
		float x = cosf(m_param.radXZ);
		float y = sinf(m_param.radY);
		float z = sinf(m_param.radXZ);

		m_param.armVec = Vec3(x, y, z);
	}

	void SeekTPS::OnUpdate2() {
		auto setPosition = CalcuMovePosition();
		auto objTrans = GetGameObject()->GetComponent<Transform>();
		objTrans->SetPosition(setPosition);
	}

	Vec3 SeekTPS::CalcuMovePosition() {
		InputYVec();
		InputRXVec();

		auto delta = App::GetApp()->GetElapsedTime();
		auto obj = GetGameObject();
		auto objTrans = obj->GetComponent<Transform>();
		auto targetTrans = m_target->GetComponent<Transform>();
		auto targetPos = targetTrans->GetWorldPosition();

		auto setPosition = targetPos + (m_param.armVec.normalize() * m_param.armRange);
		switch (m_param.seekType) {
		case SeekType::Lerp: //Lerpの時はLerpに変更
			setPosition = Lerp::CalculateLerp(objTrans->GetPosition(), setPosition, 0, 1.0f, m_param.lerpSpeed * delta, Lerp::rate::Linear);
			break;
		}

		return setPosition;
	}
	
	void SeekTPS::InputYVec()   //y軸関係の処理
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

	void SeekTPS::InputRXVec()    //x軸関係の処理
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


	void SeekTPS::CheckRimitY()
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