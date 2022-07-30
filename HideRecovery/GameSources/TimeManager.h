/*!
@file TimeManager.h
@brief ���ԊǗ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// TimeManager�ɓn���f�[�^�\����
	//-----------------------------------------------------------------------------------------------
	struct TimeData
	{
		wstring timerName; //! �^�C�}�[�̖��O
		float time; //! �c�莞��
		float endTime; //! �I������
		std::function<void()> func; //! �I�����Ɏ��s����֐�
		bool* flag; //! �I�����ɔ��]����t���O

		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �R���X�g���N�^
		* @param[in] timerName �^�C�}�[�����ʂ��邽�߂̖��O
		* @param[in] endtime �^�C�}�[�I���܂ł̕b��
		* @param[in] func �I�����Ɏ��s����֐�
		*/
		TimeData(const wstring& timerName, float endTime, std::function<void()> func)
			: timerName(timerName), time(endTime), endTime(endTime), func(func), flag(NULL)
		{}

		/**
		* @brief �R���X�g���N�^
		* @param[in] timerName �^�C�}�[�����ʂ��邽�߂̖��O
		* @param[in] endTime �^�C�}�[�I���܂ł̕b��
		* @param[in] flag �I�����ɔ��]����ϐ�
		*/
		TimeData(const wstring& timerName, float endTime, bool* flag)
			: timerName(timerName), time(endTime), endTime(endTime), func(), flag(flag)
		{}
		//-----------------------------------------------------------------------------------------------
		/// �^�C�}�[�I�����̏���
		virtual void EndTimer();
		//-----------------------------------------------------------------------------------------------
	};

	//-----------------------------------------------------------------------------------------------
	/// ���Ԍo�߂𔺂��s�����Ǘ�����R���|�[�l���g�N���X
	//-----------------------------------------------------------------------------------------------
	class TimeManager : public Component
	{
		std::list<TimeData> m_datas; //! �Ǘ����̃^�C�}�[

	public:
		//-----------------------------------------------------------------------------------------------
		/// �R���X�g���N�^
		TimeManager(const std::shared_ptr<GameObject>& ptrObj)
			:Component(ptrObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnUpdate() override; //�X�V
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief �^�C�}�[�̒ǉ�
		* @param[in] data �ǉ�����^�C�}�[�̃f�[�^
		* @return �ǉ��ɐ���������true�A���s������false
		*/
		bool AddTimer(const TimeData& data);

		/**
		* @brief �^�C�}�[�̍Ē�`
		* @param[in] data �Ē�`����^�C�}�[�̃f�[�^
		* @datail �������O�̃^�C�}�[�̃p�����[�^������������B
		* @datail �������O�̃^�C�}�[���Ȃ��ꍇ�͐V���ɒǉ�����B
		*/
		void ReSetTimer(const TimeData& data);

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------
		/// �^�C�}�[�̎c�莞�Ԃ𓾂�
		float GetTime(const wstring& str);

		/// �c�莞�Ԃ��S�̂̉�%���𓾂�
		float GetTimeRate(const wstring& str);
		//-----------------------------------------------------------------------------------------------
	};
}