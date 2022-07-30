/*!
@file SlowTimerManager.h
@brief SlowTimerManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// �X���[�^�C�}�[�p�����[�^
	//--------------------------------------------------------------------------------------
	struct SlowTimeParametor
	{
		float time;      //�X���[�ɂ��鎞��
		float timeSpeed; //�X���[�X�s�[�h

		SlowTimeParametor();
		SlowTimeParametor(const float& time, const float& timeSpeed);
	};

	//--------------------------------------------------------------------------------------
	/// �^�C���J�E���g�̃X�s�[�h�^�C�v
	//--------------------------------------------------------------------------------------
	enum class SlowTimerSpeedType {
		Normal, //�ʏ�J�E���g
		Lerp,   //�������̂��ߎg�p�֎~
	};

	//--------------------------------------------------------------------------------------
	/// �X���[�^�C�}�[�Ǘ�
	//--------------------------------------------------------------------------------------
	class SlowTimerManager : public maru::SingletonComponent<SlowTimerManager>
	{
	public:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			using SpeedType = SlowTimerSpeedType;

			float defaultTimeSpeed = 1.0f;  //�Q�[���̒ʏ�Đ����x
			SlowTimeParametor timeParam;    //�X���[�^�C���p�����[�^
			SpeedType speedType;            //�Đ��X�s�[�h�^�C�v

			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="defaultTimeSpeed">�ʏ�Đ����x</param>
			/// <param name="timeParam">�X���[�^�C���p�����[�^</param>
			Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="defaultTimeSpeed">�ʏ�Đ����x</param>
			/// <param name="timeParam">�X���[�^�C���p�����[�^</param>
			/// <param name="speedType">�Đ��X�s�[�h�^�C�v</param>
			Parametor(const float& defaultTimeSpeed, const SlowTimeParametor& timeParam, const SpeedType& speedType);
		};

	private:
		Parametor m_param;                  //�p�����[�^
		std::unique_ptr<GameTimer> m_timer; //�^�C���Ǘ��N���X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SlowTimerManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �Đ����x��Ԃ�
		/// </summary>
		/// <returns>�Đ����x</returns>
		float CalculateUpdateTimeSpeed();

		/// <summary>
		/// �ʏ�̍Đ����x��Ԃ�
		/// </summary>
		/// <returns>�ʏ�̍Đ����x</returns>
		float CalculateNormalTimerSpeed();

		/// <summary>
		/// �⊮���ꂽ�Đ����x��Ԃ�
		/// </summary>
		/// <returns>�⊮���ꂽ�Đ����x</returns>
		float CalculateLerpTimerSpeed();

		/// <summary>
		/// �^�C���A�b�v���ɌĂяo������
		/// </summary>
		void EndTime();
		
	public:
		/// <summary>
		/// �X���[�^�C���X�^�[�g
		/// </summary>
		/// <param name="time">�X���[�ɂ��鎞��</param>
		/// <param name="slowTimeSpeed">�X���[���̃X�s�[�h</param>
		void StartTimer(const float& time, const float& slowTimeSpeed = 0.3f);

		/// <summary>
		/// �����X�g�b�v
		/// </summary>
		void ForceStop();

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// Slow��Ԃ��ǂ�����Ԃ��B
		/// </summary>
		/// <returns>Slow��ԂȂ�true</returns>
		bool IsSlow() const;

		/// <summary>
		/// �X���[�^�C���p�����[�^�̎擾
		/// </summary>
		/// <returns>�X���[�^�C���p�����[�^</returns>
		SlowTimeParametor GetSlowTimeParametor() const;

		/// <summary>
		/// �ʏ�Đ����x�̎擾
		/// </summary>
		/// <returns>�ʏ�Đ����x</returns>
		float GetDefaultTimeSpeed() const noexcept {
			return m_param.defaultTimeSpeed;
		}
	};

}