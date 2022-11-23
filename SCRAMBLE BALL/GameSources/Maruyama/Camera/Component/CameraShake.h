/*!
@file CameraShake.h
@brief CameraShake�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru{
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	�J�����V�F�C�N�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class CameraShake : public Component
	{
	public:
		/// <summary>
		/// �J�����V�F�C�N�̃p�����[�^
		/// </summary>
		struct Parametor
		{
			float time;				   //�h�炷����
			Vec3 powerVec;			   //�h�炷�͂̕���
			float powerf;              //�h�炷��
			float updateTimeSpeed;     //�v�����Ԃ̃X�s�[�h
			maru::DeltaType deltaType; //�v�����ԃ^�C�v
			Vec3 centerPosition;       //���S�̏ꏊ
			std::function<void()> exitFunction = nullptr;	//�I�����ɌĂяo����������

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�h�炷����</param>
			/// <param name="powerVec">�h�炷�͂̕���</param>
			/// <param name="powerf">�h�炷��</param>
			/// <param name="updateTimeSpeed">�v�����Ԃ̃X�s�[�h</param>
			/// <param name="deltaType">�v�����ԃ^�C�v</param>
			Parametor(const float& time,
				const Vec3& powerVec,
				const float& powerf = 0.25f,
				const float& updateTimeSpeed = 1.0f,
				const maru::DeltaType& deltaType = maru::DeltaType(0)
			);
		};

	private:
		Parametor m_param;             //�p�����[�^

		ex_weak_ptr<Camera> m_camera;  //�J�����̃|�C���^

		unique_ptr<GameTimer> m_timer; //���Ԍv���N���X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		CameraShake(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate2() override;

	private:
		/// <summary>
		/// �h�炷�X�V����
		/// </summary>
		void ShakeUpdate();

	public:
		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		void StartShake(const float& time);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		/// <param name="powerVec">�h�炷����</param>
		void StartShake(const float& time, const Vec3& powerVec);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		/// <param name="powerVec">�h�炷����</param>
		/// <param name="powerf">�h�炷��</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		/// <param name="powerVec">�h�炷����</param>
		/// <param name="powerf">�h�炷��</param>
		/// <param name="updateTimeSpeed">���Ԍv���X�s�[�h</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf, const float& updateTimeSpeed);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		/// <param name="powerVec">�h�炷����</param>
		/// <param name="powerf">�h�炷��</param>
		/// <param name="updateTimeSpeed">���Ԍv���X�s�[�h</param>
		/// <param name="deltaType">���Ԍv���^�C�v</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf, const float& updateTimeSpeed, const maru::DeltaType& deltaType);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="time">�h�炷����</param>
		/// <param name="deltaType">���Ԍv���^�C�v</param>
		void StartShake(const float& time, const maru::DeltaType& deltaType);

		/// <summary>
		/// �J�����h�炵�J�n
		/// </summary>
		/// <param name="parametor">�J�����h�炵�p�����[�^</param>
		void StartShake(const Parametor& parametor);

		/// <summary>
		/// �I�����ɌĂяo������������ݒ�
		/// </summary>
		/// <param name="exitFunction"></param>
		void SetExitFunction(const std::function<void()>& exitFunction) { m_param.exitFunction = exitFunction; }

	};

}

//endbasecross