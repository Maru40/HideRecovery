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
		std::weak_ptr<T> value;        //�{�̃f�[�^

		maru::Action<void()> m_action; //�Ăяo���C�x���g�Q

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="ptr">�{�̃f�[�^�̃|�C���^</param>
		ReactiveProperty(const std::shared_ptr<T>& ptr) {
			value = ptr;
		}

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
		/// �{�̃f�[�^�̃|�C���^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̃|�C���^</returns>
		std::shared_ptr<T> GetValue() const {
			return value.lock();
		}

		/// <summary>
		/// �{�̃f�[�^�̃|�C���^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̃|�C���^</returns>
		std::shared_ptr<T> GetShared() const {
			return value.lock();
		}

		/// <summary>
		/// �{�̃f�[�^�̐��|�C���^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̐��|�C���^</returns>
		T* get() const {
			return value.lock().get();
		}

		//--------------------------------------------------------------------------------------
		///	�I�y���[�^
		//--------------------------------------------------------------------------------------

		std::shared_ptr<T> operator = (const std::shared_ptr<T> t) {
			if (this->value.lock() != t) { //�Ⴄ�̂Ȃ�
				m_action.Invoke();
			}

			this->value = t;
			return this->value.lock();
		}

		/// <summary>
		/// �{�̃f�[�^�̃|�C���^���擾
		/// </summary>
		/// <returns>�{�̃f�[�^�̃|�C���^</returns>
		std::shared_ptr<T> operator -> () const {
			return value.lock();
		}

		bool operator == (const std::shared_ptr<T>& other) const {
			return get() == other.get();
		}

		bool operator == (const ex_weak_ptr<T>& other) const {
			return get() == other.get();
		}

		bool operator != (const std::shared_ptr<T>& other) const {
			return !(get() == other.get());
		}
		bool operator != (const ex_weak_ptr<T>& other) const {
			return !(get() == other.get());
		}

		operator bool() const {
			return !value.expired();
		}

	};
}