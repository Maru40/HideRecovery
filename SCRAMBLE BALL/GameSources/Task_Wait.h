/*!
@file Task_Wait.h
@brief Task_Wait�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru {
		enum class DeltaType;
	}

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �ҋ@�^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct Wait_Parametor
		{
			float time;                   //�ҋ@���鎞��
			float timeUpdateSpeed;        //���Ԍv�����x
			maru::DeltaType deltaType;    //���Ԍv���^�C�v
			std::function<void()> start;  //�J�n���ɌĂяo���C�x���g
			std::function<bool()> update; //�X�V���ɌĂяo���C�x���g
			std::function<void()> exit;   //�I�����ɌĂяo���C�x���g

			Wait_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ҋ@����</param>
			Wait_Parametor(const float& time);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ҋ@����</param>
			/// <param name="timeUpdateSpeed">���Ԍv�����x</param>
			/// <param name="deltaType">���Ԍv���^�C�v</param>
			Wait_Parametor(const float& time, const float& timeUpdateSpeed, const maru::DeltaType& deltaType);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ҋ@����</param>
			/// <param name="start">�J�n���C�x���g</param>
			/// <param name="update">�X�V���C�x���g</param>
			/// <param name="exit">�I�����C�x���g</param>
			Wait_Parametor(const float& time,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ҋ@����</param>
			/// <param name="timeUpdateSpeed">���Ԍv�����x</param>
			/// <param name="deltaType">���Ԍv���^�C�v</param>
			/// <param name="start">�J�n���C�x���g</param>
			/// <param name="update">�X�V���C�x���g</param>
			/// <param name="exit">�I�����C�x���g</param>
			Wait_Parametor(const float& time,
				const float& timeUpdateSpeed, 
				const maru::DeltaType& deltaType,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			);
		};

		//--------------------------------------------------------------------------------------
		/// �ҋ@�^�X�N
		//--------------------------------------------------------------------------------------
		class Wait : public I_TaskNode
		{
		public:
			using Parametor = Wait_Parametor;

		private:
			std::shared_ptr<Parametor> m_paramPtr;  //�p�����[�^

			std::unique_ptr<GameTimer> m_timer;     //�^�C�}�[�Ǘ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ҋ@����</param>
			Wait(const float& time);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="parametor">�p�����[�^</param>
			Wait(const Parametor& parametor);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="paramPtr">�p�����[�^�̃|�C���^</param>
			Wait(const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}
	
}