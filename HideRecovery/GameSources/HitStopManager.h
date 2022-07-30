/*!
@file HitStopManager.h
@brief HitStopManager
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	�q�b�g�X�g�b�v�Ǘ�
	//--------------------------------------------------------------------------------------
	class HitStopManager : public maru::SingletonComponent<HitStopManager>
	{
		using SingletonComponent = maru::SingletonComponent<HitStopManager>;

	public:
		/// <summary>
		/// �q�b�g�X�g�b�v�ň����f�[�^
		/// </summary>
		struct Data 
		{
			float stopTime;     //��~���鎞��
			float startTime;    //���b��ɊJ�n���邩

			Data();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stopTime">��~���鎞��</param>
			/// <param name="startTime">���b��ɊJ�n���邩</param>
			Data(const float& stopTime, const float& startTime);
		};

	private:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			Data hitStopData;                   //�q�b�g�X�g�b�v�f�[�^
			float defaultSpeed;                 //�ʏ펞�̃^�C�}�[���x
			std::function<void()> endFunction;  //�I�����ɌĂяo���C�x���g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="hitStopData">�q�b�g�X�g�b�v�f�[�^</param>
			/// <param name="defaultSpeed">�ʏ펞�̃^�C�}�[���x</param>
			/// <param name="endFunction">�I�����ɌĂяo���C�x���g</param>
			Parametor(const Data& hitStopData, const float& defaultSpeed, const std::function<void()>& endFunction = nullptr);
		};

		Parametor m_param;					//�p�����[�^

		std::unique_ptr<GameTimer> m_timer;	//�^�C���Ǘ��N���X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		HitStopManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		/// <summary>
		/// �q�b�g�X�g�b�v�J�n
		/// </summary>
		/// <param name="stopTime">�X�g�b�v�����鎞��</param>
		void StartStop(const float& stopTime);

		/// <summary>
		/// �J�E���g�I�����ɌĂяo������(�q�b�g�X�g�b�v�I��)
		/// </summary>
		void EndTimer(); 

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �q�b�g�X�g�b�v�J�n���O������Ăяo��
		/// </summary>
		/// <param name="stopTime">�X�g�b�v�����鎞��</param>
		/// <param name="startTime">�q�b�g�X�g�b�v���J�n���鎞��</param>
		/// <param name="endFunction">�I�����ɌĂяo���C�x���g</param>
		void StartTimer(const float& stopTime, const float& startTime = 0.001f, const std::function<void()>& endFunction = nullptr);

		/// <summary>
		/// �q�b�g�X�g�b�v�f�[�^�̎擾
		/// </summary>
		/// <returns>�q�b�g�X�g�b�v�f�[�^</returns>
		Data GetStopData() const noexcept;

		/// <summary>
		/// �q�b�g�X�g�b�v�����Ă��邩�ǂ���
		/// </summary>
		/// <returns>�q�b�g�X�g�b�v���Ȃ�true</returns>
		bool IsHitStop() const;
	};

}

//endbasecross