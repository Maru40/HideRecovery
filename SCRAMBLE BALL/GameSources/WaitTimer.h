
/*!
@file WaitTimer.h
@brief WaitTimer�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	using namespace maru;

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	�ҋ@���ԃp�����[�^
	//--------------------------------------------------------------------------------------
	struct WaitTimerParametor
	{
		float time = 1.0f;							//�҂���
		float timeElapsed = 0;						//���Ԍo��
		float countSpeed = 1.0f;					//�X�V���ԑ��x
		DeltaType deltaType = DeltaType(0);			//�v�����ԃ^�C�v
		std::function<void()> endAction = nullptr;  //�I�����C�x���g
		bool isEnd = false;							//�I�����Ă��邩�ǂ���

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="time">�҂���</param>
		/// <param name="countSpeed">�X�V���ԑ��x</param>
		/// <param name="deltaType">�v�����ԃ^�C�v</param>
		/// <param name="endAction">�I�����C�x���g</param>
		WaitTimerParametor(const float& time, const float& countSpeed,
			const DeltaType& deltaType = DeltaType(0),
			const std::function<void()>& endAction = nullptr
		) :
			time(time), deltaType(deltaType), countSpeed(countSpeed), endAction(endAction)
		{}

		/// <summary>
		/// �^�C���I�����ɂ��鏈��
		/// </summary>
		/// <param name="isEndAction">�I�����C�x���g���Ăяo�����ǂ���</param>
		void EndTimer(const bool isEndAction = true) {
			timeElapsed = time;
			isEnd = true;

			if (isEndAction) {
				if (endAction) {
					endAction();
				}
			}

			endAction = nullptr;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�ҋ@���ԃR���|�[�l���g
	//--------------------------------------------------------------------------------------
	class WaitTimer : public Component
	{
		std::map<type_index, std::shared_ptr<WaitTimerParametor>> m_params;	//�p�����[�^�z��

		/// <summary>
		/// ���ԍX�V
		/// </summary>
		void TimerUpdate();

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		WaitTimer(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		void OnUpdate() override;

		/// <summary>
		/// �����I�ɑҋ@��Ԃ��������鎞�Ɏg�p����֐�
		/// </summary>
		/// <param name="tPtr">�ҋ@�������s������class�̃|�C���^</param>
		/// <param name="time">�ҋ@���鎞��</param>
		/// <param name="endAction">�I�����ɌĂяo���֐�</param>
		/// <param name="countSpeed">���Ԃ��J�E���g����X�s�[�h</param>
		template<class T>
		void AddTimer(const std::shared_ptr<T> tPtr,
			const float& time, 
			const std::function<void()>& endAction = nullptr, 
			const float& countSpeed = 1.0f,
			const maru::DeltaType & deltaType = DeltaType(0))
		{
			auto tIndex = type_index(typeid(tPtr));

			auto newData = make_shared<WaitTimerParametor>(time, countSpeed, deltaType, endAction);
			m_params[tIndex] = newData;
		}

		/// <summary>
		/// �ҋ@��Ԃ����f���ĕԂ��B
		/// </summary>
		/// <param name="tPtr">�ҋ@�������s������class�̃|�C���^</param>
		template<class T>
		bool IsWait(const std::shared_ptr<T>& tPtr) 
		{
			auto tIndex = type_index(typeid(tPtr));

			//�����̃^�C�v���ݒ肳��Ă�����
			if (m_params.count(tIndex) != 0) {
				return !m_params[tIndex]->isEnd; //�I����ԂłȂ��Ȃ�true(�ҋ@���)
			}
			else {
				return false; //�o�^����ĂȂ��Ȃ炻�������ҋ@���Ȃ��B
			}
		}

		/// <summary>
		/// �����I�ɑҋ@��Ԃ��������鎞�Ɏg�p����֐�
		/// </summary>
		/// <param name="tPtr">�ҋ@�������s������class�̃|�C���^</param>
		/// <param name="isEndAction">�I���֐����Ăяo�����ǂ���</param>
		template<class T>
		void AbsoluteEndTimer(const std::shared_ptr<T>& tPtr, const bool isEndAction) 
		{
			auto tIndex = type_index(typeid(tPtr));

			if (m_params.count(tIndex) == 0) { //�ݒ肳��Ă��Ȃ�������
				return;  //���������Ȃ��B
			}

			m_params[tIndex]->EndTimer(isEndAction);
		}

	};

}

//endbasecross