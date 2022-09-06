/*!
@file TimerDestroy.h
@brief TimerDestroy
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	//--------------------------------------------------------------------------------------
	///	��莞�Ԍ�ɍ폜���Ă����R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class TimerDestroy : public Component
	{
		unique_ptr<GameTimer> m_timer; //���ԊǗ��N���X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="time">�폜����������</param>
		/// <param name="endFunction">�I�����ɌĂяo�������C�x���g</param>
		TimerDestroy(const std::shared_ptr<GameObject>& objPtr, const float& time, const std::function<void()>& endFunction = nullptr);
		
		void OnUpdate();

	private:

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �o�ߎ��� / �ݒ莞�� == �o�ߎ��Ԃ̊���
		/// </summary>
		/// <returns>�o�ߎ��Ԃ̊���</returns>
		float GetTimeRate() const;

		/// <summary>
		/// 1.0f - ( �o�ߎ��� / �ݒ莞�� )
		/// </summary>
		/// <returns>1.0f - ( �o�ߎ��� / �ݒ莞�� )</returns>
		float GetIntervalTimeRate() const;

	};

}

//endbasecross