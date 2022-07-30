/*!
@file SeekTPS.cpp
@brief SeekTPS�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "SeekTPS.h"

namespace basecross {

	//�p�����[�^----------------------------------------------------------------------------------

	SeekTPS::Parametor::Parametor()
		:Parametor(
			20.0f,                           //�A�[�������W
			Vec3(0.0f, 0.5f, 0.0f),          //�A�[���̏�������
			XMConvertToRadians(24.0f),       //������]
			XMConvertToRadians(-90.0f),      //������]�ʒu
			0.0f,                            //Y����]�X�s�[�h
			3.0f,                            //Z����]�X�s�[�h
			XMConvertToRadians(70.0f),       //Y���̍ő�l
			XMConvertToRadians(-20.0f),      //Y���̍ŏ��l
			100.0f,                          //�Ǐ]�X�s�[�h
			SeekType::Lerp)                  //�Ǐ]�^�C�v
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
		case SeekType::Lerp: //Lerp�̎���Lerp�ɕύX
			setPosition = Lerp::CalculateLerp(objTrans->GetPosition(), setPosition, 0, 1.0f, m_param.lerpSpeed * delta, Lerp::rate::Linear);
			break;
		}

		return setPosition;
	}
	
	void SeekTPS::InputYVec()   //y���֌W�̏���
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

			CheckRimitY();  //�p�x����

			m_param.armVec.y = sinf(m_param.radY);
		}
	}

	void SeekTPS::InputRXVec()    //x���֌W�̏���
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
			//���]������0�ɖ߂�
			m_param.radXZ = 0.0f;
		}
	}


	void SeekTPS::CheckRimitY()
	{
		if (m_param.radY >= m_param.maxY)  //����𒴂�����
		{
			m_param.radY = m_param.maxY;
		}

		if (m_param.radY <= m_param.minY)  //�����𒴂�����
		{
			m_param.radY = m_param.minY;
		}
	}

}