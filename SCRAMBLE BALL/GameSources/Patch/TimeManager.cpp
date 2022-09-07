/*!
@file TimeManager.cpp
@brief ���ԊǗ�����
*/

#include "stdafx.h"
#include "TimeManager.h"

namespace basecross
{
	void TimeData::EndTimer()
	{
		if (func) // �֐��������Ă�����
		{
			if (func.operator bool()) // �֐������s�\���𔻒�
			{
				func(); // ���s
			}
		}

		if (flag != NULL) // �t���O�������Ă�����
		{
			*flag = !*flag; // ���]
		}
	}

	//------------------------------------------------------------------------------------------------
	void TimeManager::OnUpdate()
	{
		auto iter = m_datas.begin();
		while (iter != m_datas.end()) // �Q�Ɛ悪�Ōザ�ᖳ�����
		{
			//�c��b������f���^���Ԃ�����
			auto delta = App::GetApp()->GetElapsedTime();
			iter->time -= delta;

			//�c�莞�Ԃ�0�Ȃ�
			if (iter->time <= 0.0f)
			{
				iter->EndTimer(); //�֐��̎��s(�܂��̓t���O�̔��])

				//�^�C�}�[�̍폜
				iter = m_datas.erase(iter); // ��������
				continue;
			}
			iter++;
		}
	}

	bool TimeManager::AddTimer(const TimeData& data)
	{
		if (m_datas.size() == 0) // �Ǘ����Ă���^�C�}�[��1���Ȃ��Ȃ�
		{
			m_datas.push_back(data); // �^�C�}�[�̒ǉ�
			return true;
		}

		for (auto mData : m_datas)
		{
			if (mData.timerName == data.timerName) //�^�C�}�[�̖��O�����ɂ���Ȃ�
			{
				return false; // �ǉ������ɏI��
			}
		}
		m_datas.push_back(data); // �����̃^�C�}�[��������Βǉ�
		return true;
	}

	void TimeManager::ReSetTimer(const TimeData& data)
	{
		for (auto mData : m_datas)
		{
			if (mData.timerName == data.timerName) //�������O�̂��̂���������
			{
				mData = data; // �f�[�^���X�V
				return;
			}
		}
		m_datas.push_back(data); //����������V���ɒǉ�����
	}

	float TimeManager::GetTime(const wstring& str)
	{
		for (auto data : m_datas)
		{
			if (str == data.timerName) // �ړI�̖��O�Ɠ������O�̃^�C�}�[��T��
			{
				return data.time; // ���������玞�Ԃ�Ԃ�
			}
		}
		return 0.0f; // ������Ȃ�������0��Ԃ�
	}

	float TimeManager::GetTimeRate(const wstring& str)
	{
		for (auto data : m_datas)
		{
			if (str == data.timerName)  // �ړI�̖��O�Ɠ������O�̃^�C�}�[��T��
			{
				return data.time / data.endTime; // ����������l��Ԃ�
			}
		}
		return 0.0f; // ������Ȃ�������0��Ԃ�
	}
};