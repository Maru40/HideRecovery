/*!
@file ReactiveProperty.h
@brief ReactiveProperty
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���A�N�e�B�u�v���p�e�B
	//--------------------------------------------------------------------------------------
	template<class T>
	class ReactiveProperty 
	{
	private:
		T value;						//�{�̃f�[�^

		maru::Action<void()> m_action;	//�Ăяo���C�x���g�Q

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="ptr">�{�̃f�[�^</param>
		ReactiveProperty(const T& value):
			value(value)
		{}

		virtual ~ReactiveProperty() {};

		/// <summary>
		/// �؂�ւ������ɌĂт�������
		/// </summary>
		/// <param name="whereAction">������</param>
		/// <param name="action">�Ăяo����������</param>
		void AddSubscribe(const std::function<bool()>& whereAction, const std::function<void()>& action) {
			m_action.AddFunction(
				[&, whereAction, action]() {
					if (whereAction()) { action(); };
				}
			);
		}

		/// <summary>
		/// �؂�ւ������ɌĂт�������
		/// </summary>
		/// <param name="whereAction">������</param>
		/// <param name="action">�Ăяo����������</param>
		void AddSubscribe(const maru::Action<bool()>& whereAction, const maru::Action<void()>& action) {
			m_action.AddFunction(
				[&, whereAction, action]() {
					if (whereAction.Invoke()) { action.Invoke(); };
				}
			);
		}

		/// <summary>
		/// �{�̃f�[�^�̐ݒ�
		/// </summary>
		/// <param name="value">�{�̃f�[�^</param>
		void SetValue(const T& value) { this->value = value; }

		/// <summary>
		/// �{�̃f�[�^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^</returns>
		T GetValue() const noexcept { return value; }

		/// <summary>
		/// �{�̃f�[�^�̎Q�Ƃ��擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̎Q��</returns>
		T& GetRefValue() noexcept { return value; }

		/// <summary>
		/// �{�̃f�[�^�̐��|�C���^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̐��|�C���^</returns>
		T* get() const { return &value; }

		//--------------------------------------------------------------------------------------
		///	�I�y���[�^
		//--------------------------------------------------------------------------------------

		T operator = (const T t) {
			if (&GetValue() != &t) { //�Ⴄ�̂Ȃ�
				m_action.Invoke();
			}

			this->value = t;
			return this->value;
		}

		bool operator == (const T& other) const {
			return &GetValue() == &other;
		}

		bool operator != (const T& other) const {
			return !(&GetValue() == &other);
		}

	};
}